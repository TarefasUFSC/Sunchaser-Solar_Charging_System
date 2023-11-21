import React, {useState} from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';

import {HorizontalList, RoundButton} from '../components/components';

import { getSettings, setSettings } from '../routes/routes';

function Settings({ navigation }) {
  data = /*getSettings().settings*/ [1, 2, 3]; 
  const [readingInterval, setReadingInterval] = useState(data[0]);
  const [sendingInterval, setSendingInterval] = useState(data[1]);
  const [daysBackup, setDaysBackup] = useState(data[2]);

  function teste(){
    setReadingInterval(20);
    setSendingInterval(2);
    setDaysBackup(50);
  }

  return (
    <View style={{ backgroundColor: 'white', flex: 1 }}><ScrollView>
        <View><HorizontalList Variavel="Intervalo entre leituras" Unidade=" minutos" Valor={readingInterval} Input={true}/></View>
        <View><HorizontalList Variavel="Intervalo entre envios" Unidade=" horas" Valor={sendingInterval} Input={true}/></View>
        <View><HorizontalList Variavel="Dias estocados na memoria" Unidade=" dias" Valor={daysBackup} Input={true}/></View>
        <RoundButton palavra='Salvar' color = '#5DB075' tColor='white' onPressFunction={()=>teste()/*setSettings(5,1,10)*/}/>
    </ScrollView></View>
  );
}

const styles = StyleSheet.create(

)

export default Settings;
