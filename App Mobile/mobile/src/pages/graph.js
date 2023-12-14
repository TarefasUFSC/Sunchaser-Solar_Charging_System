import React, { useEffect, useState, useContext } from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';

import { Chart, Dropdown } from '../components/components';
import { ESP32Context } from '../../App'; // Importar o contexto

function Graph() {
	const { batVolt, solarBatAmp, batLoadAmp, reloadData, checkConnection } = useContext(ESP32Context);
	const [selectedOption, setSelectedOption] = useState('Tensão na Bateria');
	const [chartData, setChartData] = useState(null);

	useEffect(() => {
		changeChartData(batVolt, 'V')
	}, []);

	function changeChartData(data_list, unit) {

		checkConnection();
		let inverted_data_list = data_list.reverse()
		let newChartData = inverted_data_list.map(it => {
			const date = it['datetime'].split('T')[0].replaceAll('-', '/').substr(2, it.length).split('/')
			let dt = {
				'value': it['value'],
				'label': date[2] + '/' + date[1],
				'unit': unit
			}
			return dt;
		})
		setChartData(newChartData)
	}

	const handleOptionChange = (option) => {
		setSelectedOption(option);
		switch (option) {
			case 'Tensão na Bateria':
				changeChartData(batVolt, 'V')
				break;
			case 'Corrente entre o Painel e a Bateria':
				changeChartData(solarBatAmp, 'A')
				break;
			case 'Corrente entre a Bateria e a Carga':
				changeChartData(batLoadAmp, 'A')
				break;
			default:
				changeChartData(batVolt, 'V')
		}
	};

	return (
		<ScrollView style={{ backgroundColor: 'white', flex: 1 }} vertical>
			<View style={styles.graphContainer}>
				<View style={styles.topContainer}>
					<View style={styles.leftContent} />
					<Dropdown onOptionChange={handleOptionChange} />
				</View>
				{chartData ?
					<View style={styles.chartContainer}>
						<ScrollView horizontal><Chart data={chartData} opt={selectedOption} /></ScrollView>
					</View> :
					<Text>Sem Dados</Text>}
			</View>
		</ScrollView>
	);
}

const styles = StyleSheet.create({
	graphContainer: {
		flexDirection: 'column',
		backgroundColor: 'white',
	},
	topContainer: {
		height: '80',
		flexDirection: 'row',
		justifyContent: 'space-between',
	},
	leftContent: {
		flexDirection: 'row',
		alignItems: 'center',
	},
	chartContainer: {
		backgroundColor: 'white',
	},
})

export default Graph;
