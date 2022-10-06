import axios from 'axios';

export const http = axios.create({
    baseURL: 'https://lixeira-iot.herokuapp.com/api/'
})