import React from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';

import {HorizontalList, HR, RoundButton} from '../components/components';

function Settings({ navigation }) {
  return (
    <ScrollView>
        <View><HorizontalList Variavel="Intervalo entre leituras" Valor=" minutos" Input={true}/><HR/></View>
        <View><HorizontalList Variavel="Intervalo entre envios" Valor=" horas" Input={true}/><HR/></View>
        <View><HorizontalList Variavel="Dias estocados na memoria" Valor=" dias" Input={true}/></View>
        <RoundButton palavra='salvar'/>
    </ScrollView>
  );
}

const styles = StyleSheet.create(

)

export default Settings;
