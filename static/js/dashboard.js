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
            console.log(http.get('pessoas/'))
            
        }
        
    }
})