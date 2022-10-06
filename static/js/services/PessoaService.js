import { http } from "./axios";

export default {

    listar:() => {
        return http.get('pessoas/')
    },

    salvar:(pessoa) => {
        return http.post('pessoas/create/', pessoa)
    },

}