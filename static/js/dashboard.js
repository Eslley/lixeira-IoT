const http = axios.create({
    baseURL: 'https://lixeira-iot.herokuapp.com/api/',
})

var v = new Vue({
    el: '#content',
    delimiters: ['${', '}'],

    data: {
        pessoas: [],
        estado: "Fechado",
        nivel: 0,
        nome: ""
    },

    mounted() {
        this.listar()
    },
    
    watch:{
    },

    methods:{

        salvar() {
            http.post('pessoas/create/', { 'nome': this.nome }).then(response => {
                if (response.status == 201) {
                    alert("Adicionado com sucesso!")

                    this.listar()
                } else {
                    alert(response.data.message)
                }
            }).catch(err => {
                this.showAlert("error", "Erro", "A pessoa não foi cadastrada")
                console.log(err)
            })
        },

        listar() {
            http.get('pessoas/').then(response => {
                this.pessoas = response.data
            })
        },

        sortear() {
            if (this.pessoas.length <= 0) {
                this.showAlert("warning", "Aviso", "Sem pessoas para sortear!")
            } else {
                const index = Math.floor(Math.random() * this.pessoas.length)
                this.showAlert("success", "Sorteio", "Sorteado foi " + this.pessoas[index].nome)
            }
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