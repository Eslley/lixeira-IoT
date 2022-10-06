const http = axios.create({
    baseURL: 'https://lixeira-iot.herokuapp.com/api/',
})

var v = new Vue({
    el: '#content',
    delimiters: ['${', '}'],
    data: {
        pessoas: []
    },
    watch:{
    },

    methods:{

        salvar() {

        },

        listar() {
            http.get('pessoas/').then(response => {
                pessoas = response.data
                console.log(response)
            })
            
        }
        
    }
})