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
        }
        
    }
})