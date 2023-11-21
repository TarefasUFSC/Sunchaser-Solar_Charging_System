import axios from 'axios';
import React, { useEffect, useState } from 'react';

export function reading() {
	const [data, setData] = useState([]);
	axios.post('192.168.1.1/reading')
		.then(response => {
			setData(response.data);
		})
		.catch(error => {
			console.error('Erro na chamada GET:', error);
		});
	return data;
}

export function setSettings(leitura, envio, dias) {
	const data = {
		settings: {
			"iLeitura": leitura,
			"iEnvio": envio,
			"dias": dias
		}
	}
	axios.put('192.168.1.1/settings', data)
		.then(response => {
			console.log('Resposta: ', response.data);
			alert('Configurações enviadas');
		})
		.catch(error => {
			console.error('Erro na chamada PUT:', error);
			alert('Deu pau. PERNA ARRUMA ISSO LOGO');
		});
}

export function getSettings() {
	const [data, setData] = useState([]);
	axios.get('192.168.1.1/settings')
		.then(response => {
			setData(response.data);
		})
		.catch(error => {
			console.error('Erro na chamada GET: ', error);
		});
	return data;
}



export function getLTM() {
	axios.get('http://192.168.1.1/ltm?page=1')
		.then(response => {
			return (response.data);
		})
		.catch(error => {
			console.error('Erro na chamada GET: ', error);
			return { "error": error }
		});
}
