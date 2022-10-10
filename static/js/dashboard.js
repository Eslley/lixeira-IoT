const threshold = 30;

const http = axios.create({
    baseURL: 'https://lixeira-iot.herokuapp.com/api/',
})

var v = new Vue({
    el: '#content',
    delimiters: ['${', '}'],

    data: {
        lixeiras: [],
        pessoas: [],
        nome: ""
    },

    mounted() {
        this.listarlixeiras()
        this.listar()
        
    },
    
    watch:{
    },

    methods:{

        salvar() {
            http.post('pessoas/create/', { 'nome': this.nome }).then(response => {
                if (response.status == 201) {
                    this.showAlert("success", "Aviso", "A pessoa foi adicionada com sucesso!")
                    this.listar()
                } else {
                    this.showAlert("error", "Erro", response.data.message)
                }
            }).catch(err => {
                this.showAlert("error", "Erro", "A pessoa não foi cadastrada")
                console.log(err)
            })
        },

        listarlixeiras(){
            http.get('lixeiras?nivel[>e]='+threshold).then(response => {
                this.lixeiras = response.data;
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
                const index = this.getRandomInt(0, this.pessoas.length)
                const indexl = this.getRandomInt(0, this.lixeiras.length);
                this.showAlert("success", "Sorteio", "Sorteado foi " + this.pessoas[index].nome + 
                " para a lixeira "+this.lixeiras[indexl].id)
            }
        },

        getRandomInt(min, max) {
            min = Math.ceil(min);
            max = Math.floor(max);
            return Math.floor(Math.random() * (max - min) + min);
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