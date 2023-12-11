import React, { useState, useEffect } from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';
import axios from 'axios';

import { HorizontalList, RoundButton } from '../components/components';

import { getSettings, setSettings } from '../routes/routes';

function Settings({ navigation }) {
  data = /*getSettings().settings*/[1, 2, 3];
  const [readingInterval, setReadingInterval] = useState('');
  const [cacheMaxSize, setCacheMaxSize] = useState('');
  const [ltmMaxSize, setLtmMaxSize] = useState('');

  useEffect(() => {

    getSettings()
  }, []);



  function setSettings(leitura, envio, dias) {
    const data = {
      "readingInterval": parseInt(leitura),
      "cacheMaxSize": parseInt(envio),
      "ltmMaxSize": parseInt(dias)
    }
    axios.put('http://192.168.1.1/settings', data)
      .then(response => {
        console.log('Resposta: ', response.data);
        alert('Configurações enviadas');
        setReadingInterval(parseInt(leitura))
        setCacheMaxSize(parseInt(envio))
        setLtmMaxSize(parseInt(dias))
      })
      .catch(error => {
        console.error('Erro na chamada PUT:', error);
        alert('Deu pau. PERNA ARRUMA ISSO LOGO');
      });
  }

  function getSettings() {
    axios.get('http://192.168.1.1/settings')
      .then(response => {
        console.log(response.data);

        console.log(response.data['readingInterval'], response.data['cacheMaxSize'], response.data['ltmMaxSize']);
        setReadingInterval(parseInt(response.data['readingInterval']))
        setCacheMaxSize(parseInt(response.data['cacheMaxSize']))
        setLtmMaxSize(parseInt(response.data['ltmMaxSize']))

        console.log(response.data['readingInterval'], cacheMaxSize, ltmMaxSize);
      })
      .catch(error => {
        console.error('Erro na chamada GET: ', error);
        setReadingInterval(null)
        setCacheMaxSize(null)
        setLtmMaxSize(null)
      });
    return data;
  }
  return (
    <View style={{ backgroundColor: 'white', flex: 1 }}><ScrollView>
      {readingInterval != null && cacheMaxSize != null && ltmMaxSize != null ?
        <>
          <View><HorizontalList Variavel="Intervalo entre leituras" Unidade=" minutos" Valor={readingInterval} setValor={setReadingInterval} Input={true} /></View>
          <View><HorizontalList Variavel="Envios salvos no cache" Unidade=" leituras" Valor={cacheMaxSize} setValor={setCacheMaxSize} Input={true} /></View>
          <View><HorizontalList Variavel="Leituras estocadas na memoria" Unidade=" leituras" Valor={ltmMaxSize} setValor={setLtmMaxSize} Input={true} /></View>
          <RoundButton palavra='Salvar' color='#5DB075' tColor='white' onPressFunction={() => setSettings(readingInterval, cacheMaxSize, ltmMaxSize)/*setSettings(5,1,10)*/} />
        </> : <Text>Sem conexão com o ESP para pegar as informações das conigurações dele</Text>}
    </ScrollView></View>
  );
}

const styles = StyleSheet.create(

)

export default Settings;
