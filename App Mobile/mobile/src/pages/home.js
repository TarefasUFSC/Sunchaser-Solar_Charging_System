import React from 'react';
import { View, Text, Button, ScrollView } from 'react-native';

import {HorizontalList, HR} from '../components/components';

function Home({ navigation }) {
    return (
        <ScrollView>
            <View><HorizontalList Variavel="Data" Valor="02/09/2023" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Hora" Valor="18:05:23" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Corrente Painel" Valor="3 mA" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Corrente B" Valor="2.7 mA" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Corrente C" Valor="2.6 mA" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Tensão Painel" Valor="5.7 V" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Tensão Bateria" Valor="6.3 V" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Temperatura Painel" Valor="73°C" Input={false}/><HR/></View>
            <View><HorizontalList Variavel="Temperatura Bateria" Valor="45°C" Input={false}/></View>
        </ScrollView>
    );
}
export default Home;