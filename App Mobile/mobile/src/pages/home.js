import React, { useEffect, useState } from 'react';
import { View, Text, ScrollView, StyleSheet, BackHandler } from 'react-native';
import { BleManager } from 'react-native-ble-plx';

import { HorizontalList, Teste } from '../components/components';

function Home({ navigation }) {
	return (
		<View style={styles.baseHomeContainer}><ScrollView >
			<View><HorizontalList Variavel="Data" Valor="02/09/2023" Input={false}/></View>
			<View><HorizontalList Variavel="Hora" Valor="18:05:23" Input={false}/></View>
			<View><HorizontalList Variavel="Corrente Painel" Valor="3 mA" Input={false}/></View>
			<View><HorizontalList Variavel="Corrente B" Valor="2.7 mA" Input={false}/></View>
			<View><HorizontalList Variavel="Corrente C" Valor="2.6 mA" Input={false}/></View>
			<View><HorizontalList Variavel="Tensão Painel" Valor="5.7 V" Input={false}/></View>
			<View><HorizontalList Variavel="Tensão Bateria" Valor="6.3 V" Input={false}/></View>
			<View><HorizontalList Variavel="Temperatura Painel" Valor="73°C" Input={false}/></View>
			<View><HorizontalList Variavel="Temperatura Bateria" Valor="45°C" Input={false}/></View>
		</ScrollView></View>
	);
}

const styles = StyleSheet.create({
	baseHomeContainer: {
		backgroundColor: '#FFFFFF',
		flex: 1,
	},
	bleHomeContainer: {
		backgroundColor: '#4B9460',
		flex: 1,
		flexDirection: 'column',
	},
	bleHomeTextContainer: {
		backgroundColor: '#FFFFFF',
	},
	bleHomeButtons: {
		marginTop: 20,
		flexDirection: 'row'
	},
	Button:{
		backgroundColor: '#4B9460',
    borderRadius: 10,
    paddingVertical: 10,
    paddingHorizontal: 20,
    marginTop: 20,
	}
});

export default Home;