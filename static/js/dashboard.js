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
                console.log(err)
            })
        },

        listar() {
            http.get('pessoas/').then(response => {
                this.pessoas = response.data
                console.log(response)
            })
            
        }
        
    }
})