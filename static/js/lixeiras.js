const http = axios.create({
    baseURL: 'https://lixeira-iot.herokuapp.com/api/',
})

const threshold = 30;

var v = new Vue({
    el: '#content',
    delimiters: ['${', '}'],

    data: {
        lixeiras: [],
        threshold: threshold,
    },

    mounted() {
      this.listar()

      setInterval(() => {
        this.listar()
      }, 5000)
        
    },
    
    watch:{
    },

    methods:{

        listar() {
            http.get('lixeiras/').then(response => {
                this.lixeiras = response.data;
            })
        },

        showAlert(type, title, message) {

            switch(type) {
              case "success":
                Swal.fire({
                  title: title,
                  text: message,
                  icon: 'success',
                  iconColor: '#f5f5f5',
                  timer: 4000,
                  showConfirmButton: false,
                  toast: true,
                  position: 'top',
                  color: '#f5f5f5',
                  background: '#77b93f'
                })
                break
              case "error":
                Swal.fire({
                  title: title,
                  text: message,
                  icon: 'error',
                  iconColor: '#f5f5f5',
                  timer: 4000,
                  showConfirmButton: false,
                  toast: true,
                  position: 'top',
                  color: '#f5f5f5',
                  background: '#ff4f52'
                })
                break
              case "warning":
                Swal.fire({
                  title: title,
                  text: message,
                  icon: 'warning',
                  iconColor: '#f5f5f5',
                  timer: 4000,
                  showConfirmButton: false,
                  toast: true,
                  position: 'top',
                  color: '#f5f5f5',
                  background: '#ff4f52'
                })
                break
            }
          }
        
    }
})