import React from 'react';
import { View, ScrollView, Text, Button } from 'react-native';

import {HorizontalList, HR} from '../components/components';

function Settings({ navigation }) {
  return (
    <ScrollView>
        <View><HorizontalList Variavel="Intervalo entre leituras" Valor=" minutos" Input={true}/><HR/></View>
        <View><HorizontalList Variavel="Intervalo entre envios" Valor=" horas" Input={true}/><HR/></View>
        <View><HorizontalList Variavel="Dias estocados na memoria" Valor=" dias" Input={true}/></View>
    </ScrollView>
  );
}

export default Settings;
