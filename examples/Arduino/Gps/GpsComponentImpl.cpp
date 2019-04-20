// ====================================================================== 
// \title  GpsImpl.cpp
// \author lestarch
// \brief  cpp implementation of the F' sample GPS receiver for a
//         NMEA GPS receiver device.
//
// \copyright
// Copyright 2018, lestarch.
// ====================================================================== 

#include <examples/Arduino/Gps/GpsComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <cstdlib>
#include <Arduino.h>

#include <stdint.h>
namespace Gps {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  GpsComponentImpl ::
#if FW_OBJECT_NAMES == 1
    GpsComponentImpl(
        const char *const compName
    ) :
      GpsComponentBase(compName),
#else
      GpsComponentBase(void),
#endif
      m_in_ring(m_in_buffer, GP_BUFF_SIZE),
      m_mangled(0),
      m_setup(false),
      m_locked(false)
  {
      unsigned long int my_data = 5;
      unsigned long int* my_pointer = &my_data;
      *my_pointer = 3;
  }

  void GpsComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    ) 
  {
    GpsComponentBase::init(instance);
  }

  GpsComponentImpl ::
    ~GpsComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  // Step 0: schedIn
  //
  //  By implementing this "handler" we can respond to the 1HZ call allowing
  //  us to read the GPS UART every 1 second.
  void GpsComponentImpl ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
//      static float lat =0.0f; static float lon = 0.0f; static float alt=0.0f;

      /*tlmWrite_Gps_Latitude(lat);
      tlmWrite_Gps_Longitude(lon);
      tlmWrite_Gps_Altitude(alt);
      tlmWrite_Gps_Count((int) lat);*/
      // GPS messages will come in once a second, thus we can process and drain the
      // ring buffer on a 1-second clock
      //processRing();
     /* lat += 1.0f;
      lon += 1.0f;
      alt += 1.0f;*/
  }

  void  GpsComponentImpl ::
    readCallback_handler(
        const NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::Buffer& fwBuffer /*!< Buffer*/
    )
  {
      //processBuffer(fwBuffer);
  }

  bool GpsComponentImpl :: readFloat(float& output) {
      NATIVE_UINT_TYPE size = 0;
      char* end = NULL;
      char field[12];
      Fw::SerializeStatus status = m_in_ring.peek(reinterpret_cast<U8*>(field), sizeof(field) - 1);
      field[sizeof(field) - 1] = '\0';
      output = static_cast<float>(strtod(field, &end));
      size = reinterpret_cast<POINTER_CAST>(end) - reinterpret_cast<POINTER_CAST>(&field[0]);
      // Error conditions
      if (size < 2 || output == 0.0 || status != Fw::FW_SERIALIZE_OK) {
          return false;
      }
      m_in_ring.rotate(size + 1);
      return true;
  }

  bool GpsComponentImpl :: detectGga(GgaPacket& packet) {
      // For every field in the message, the data will be peeked, converted, and then shifted away
      // Start with the UTC time
      U8 tmp8;
      m_in_ring.rotate(1);
      if (!readFloat(packet.utcTime)) {
          return false;
      }
      // Degrees minutes seconds and N/S character
      if (!readFloat(packet.dmNS)) {
          return false;
      }
      m_in_ring.peek(tmp8);
      packet.northSouth = tmp8;
      m_in_ring.rotate(2);
      // Degrees minutes seconds and E/W character
      if (!readFloat(packet.dmEW)) {
          return false;
      }
      m_in_ring.peek(tmp8);
      packet.eastWest = tmp8;
      m_in_ring.rotate(2);
      // Lock character
      m_in_ring.peek(tmp8);
      packet.lock = tmp8 - '0';
      m_in_ring.rotate(2);
      // Count is 2 character ascii digits
      m_in_ring.peek(tmp8);
      packet.count = (tmp8 - '0') * 10;
      m_in_ring.peek(tmp8, 1);
      packet.count += (tmp8 - '0');
      m_in_ring.rotate(3);
      // HDOP value
      if (!readFloat(packet.hdop)) {
          return false;
      }
      // Altitude1
      if (!readFloat(packet.altitude)) {
          return false;
      }
      return true;
  }

  void GpsComponentImpl :: processRing() {
      GgaPacket packet;
      char temp[GPGGA_LENGTH + 1];
      // Process through the entire ring buffer taking messages
      while (m_in_ring.get_remaining_size() >= GP_HEADER_LENGTH) {
          // Read routing header
          m_in_ring.peek(reinterpret_cast<U8*>(temp), GP_HEADER_LENGTH);
          temp[GP_HEADER_LENGTH] = '\0';
          m_in_ring.rotate(GP_HEADER_LENGTH);
          // Look for known headers
          if (strcmp(temp, "$GPGGA") == 0) {
              // Break if not enough data
              if (m_in_ring.get_remaining_size() < GPGGA_LENGTH) {
                  break;
              }

              // Mangled message, send telemetry
              if (!detectGga(packet)) {
                  m_mangled += 1;
                  tlmWrite_Gps_Mangled(m_mangled);
              }
              // Process GPS message
              else {
                  processGga(packet);
              }
          }
          // No header found, rotate buffer and continue
          else {
              m_in_ring.rotate(1);
          }
      }
  }
  void GpsComponentImpl :: processGga(GgaPacket& packet) {
      float lat = 0.0f, lon = 0.0f;
      //GPS packet locations are of the form: ddmm.mmmm
      //We will convert to lat/lon in degrees only before downlinking
      //Latitude degrees, add on minutes (converted to degrees), multiply by direction
      lat = (U32)(packet.dmNS/100.0f);
      lat = lat + (packet.dmNS - (lat * 100.0f))/60.0f;
      lat = lat * ((packet.northSouth == 'N') ? 1 : -1);
      //Longitude degrees, add on minutes (converted to degrees), multiply by direction
      lon = (U32)(packet.dmEW/100.0f);
      lon = lon + (packet.dmEW - (lon * 100.0f))/60.f;
      lon = lon * ((packet.eastWest == 'E') ? 1 : -1);
      // Call the downlink functions to send down data
      tlmWrite_Gps_Latitude(lat);
      tlmWrite_Gps_Longitude(lon);
      tlmWrite_Gps_Altitude(packet.altitude);
      tlmWrite_Gps_Count(packet.count);
      //Lock status update only if changed
      //Step 7,8: note changed lock status
      // Emit an event if the lock has been aquired, or lost
      if (packet.lock == 0 && m_locked) {
          m_locked = false;
          log_WARNING_HI_Gps_LockLost();
      } else if (packet.lock == 1 && !m_locked) {
          m_locked = true;
          log_ACTIVITY_HI_Gps_LockAquired();
      }
      //Serial.println("\nPacket-go  11\n");
  }

  void GpsComponentImpl :: processBuffer(Fw::Buffer& buffer)
  {
      //Serial.write(reinterpret_cast<U8*>(buffer.getdata()), buffer.getsize());
      NATIVE_UINT_TYPE buffer_offset = 0;
      while (buffer_offset < buffer.getsize()) {
          NATIVE_UINT_TYPE ser_size = (buffer.getsize() >= m_in_ring.get_remaining_size(true)) ?
              m_in_ring.get_remaining_size(true) : static_cast<NATIVE_UINT_TYPE>(buffer.getsize());
          m_in_ring.serialize(reinterpret_cast<U8*>(buffer.getdata()) + buffer_offset, ser_size);
          buffer_offset = buffer_offset + ser_size;
      }
      // Data not placed on the ring will be lost.  Thus the ring needs to be large enough to
      // hold a second worth of data.
  }

  // ----------------------------------------------------------------------
  // Command handler implementations 
  // ----------------------------------------------------------------------
  //Step 9: respond to status command
  //
  // When a status command is received, respond by emitting the 
  // current lock status as an Event.
  void GpsComponentImpl ::
    Gps_ReportLockStatus_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    //Locked-force print
    if (m_locked) {
        log_ACTIVITY_HI_Gps_LockAquired();
    } else {
        log_WARNING_HI_Gps_LockLost();
    }
  }
} // end namespace GpsApp
