const http = axios.create({
    baseURL: 'https://lixeira-iot.herokuapp.com/api/',
})

var v = new Vue({
    el: '#content',
    delimiters: ['${', '}'],

    data() {
        return {
            pessoas: [],
            estado: "Fechado",
            nivel: 0
        }
    },

    mounted() {
        this.listar()
    },
    
    watch:{
    },

    methods:{

        salvar() {

        },

        listar() {
            http.get('pessoas/').then(response => {
                this.pessoas = response.data
                console.log(response)
            })
            
        }
        
    }
})