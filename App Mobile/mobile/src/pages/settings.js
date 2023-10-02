import React from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';

import {HorizontalList, RoundButton} from '../components/components';

function Settings({ navigation }) {
  return (
    <View style={{ backgroundColor: 'white', flex: 1 }}><ScrollView>
        <View><HorizontalList Variavel="Intervalo entre leituras" Valor=" minutos" Input={true}/></View>
        <View><HorizontalList Variavel="Intervalo entre envios" Valor=" horas" Input={true}/></View>
        <View><HorizontalList Variavel="Dias estocados na memoria" Valor=" dias" Input={true}/></View>
        <RoundButton palavra='salvar' page='settings'/>
    </ScrollView></View>
  );
}

const styles = StyleSheet.create(

)

export default Settings;
