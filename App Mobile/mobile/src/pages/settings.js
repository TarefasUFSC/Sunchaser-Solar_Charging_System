import React, { useState, useEffect } from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';
import axios from 'axios';

import { HorizontalList, RoundButton } from '../components/components';

function Settings({ navigation }) {
  // data = /*getSettings().settings*/[1, 2, 3];
  const [readingInterval, setReadingInterval] = useState('');
  const [sendingInterval, setSendingInterval] = useState('');
  const [daysBackup, setDaysBackup] = useState('');

  useEffect(() => {
    getSettings()
  }, []);

  function setSettings(leitura, envio, dias) {
    if(leitura>60){
      alert("O intervalo de leitura não pode ser maior que 60 minutos");
    }
    else{
      const data = {
        "readingInterval": parseInt(leitura),
        "sendingInterval": parseInt(envio),
        "daysBackup": parseInt(dias)
      }
      axios.put('http://192.168.1.1/settings', data, {timeout: 3000})
        .then(response => {
          console.log('Resposta: ', response.data);
          alert('Configurações enviadas');
          setReadingInterval(parseInt(leitura))
          setSendingInterval(parseInt(envio))
          setDaysBackup(parseInt(dias))
        })
        .catch(error => {
          if (error.code === 'ECONNABORTED') {alert('Você não está conectado ao ESP');}
          else console.error('Erro na chamada PUT:', error);
        });
    }
  }

  function getSettings() {
    axios.get('http://192.168.1.1/settings', { timeout: 2000 })
      .then(response => {
        console.log(response.data);

        console.log(response.data['readingInterval'], response.data['sendingInterval'], response.data['daysBackup']);
        setReadingInterval(parseInt(response.data['readingInterval']))
        setSendingInterval(parseInt(response.data['sendingInterval']))
        setDaysBackup(parseInt(response.data['daysBackup']))

        console.log(response.data['readingInterval'], sendingInterval, daysBackup);
      })
      .catch(error => {
        if (error.code === 'ECONNABORTED') {alert('Você não está conectado ao ESP');}
        else {console.error('Erro na chamada GET: ', error);}
        setReadingInterval(null)
        setSendingInterval(null)
        setDaysBackup(null)
      });
  }

  return (
    <View style={{ backgroundColor: 'white', flex: 1 }}><ScrollView>
      {readingInterval != null && sendingInterval != null && daysBackup != null ?
        <>
          <View><HorizontalList Variavel="Intervalo entre leituras" Unidade=" minutos" Valor={readingInterval} setValor={setReadingInterval} Input={true} /></View>
          <View><HorizontalList Variavel="Intervalo entre envios" Unidade=" horas" Valor={sendingInterval} setValor={setSendingInterval} Input={true} /></View>
          <View><HorizontalList Variavel="Dias estocados na memoria" Unidade=" dias" Valor={daysBackup} setValor={setDaysBackup} Input={true} /></View>
          <RoundButton palavra='Salvar' color='#5DB075' tColor='white' onPressFunction={() => setSettings(readingInterval, sendingInterval, daysBackup)/*setSettings(5,1,10)*/} />
        </> : <Text>Sem conexão com o ESP para pegar as informações das configurações dele</Text>}
    </ScrollView></View>
  );
}

const styles = StyleSheet.create()

export default Settings;
