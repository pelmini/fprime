var TableView = Vue.extend({
    template:
        `<table>
          <thead>
            <td>Name</td>
            <td>Timestamp</td>
            <td>Value</td>
          </thead>
          <tbody>
            <tr v-for="row in rows">
              <td>{{ row.Name }}</td>
              <td>{{ row.Timestamp }}</td>
              <td :class="row.valueClass">{{ row.Value }}</td>
            </tr>
          </tbody>
        </table>`,
    data: function () {
        return {
            headers: [
                'Timestamp',
                'Value'
            ],
            children: []
        };
    }
});
