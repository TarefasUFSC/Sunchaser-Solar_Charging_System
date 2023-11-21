import React, { useState } from 'react';
import { View, ScrollView, StyleSheet, Text, Button } from 'react-native';
import dados1 from '../../dataTest/dados7.json';
import dados2 from '../../dataTest/dados12.json';
import dados3 from '../../dataTest/dados15.json';
import dados4 from '../../dataTest/dados30.json';
import dados5 from '../../dataTest/dados60.json';
import { Chart, Dropdown } from '../components/components';

function Graph({ navigation }) {
	const [selectedOption, setSelectedOption] = useState('Opção 1');
	const [chartData, setChartData] = useState(dados1);

	const handleOptionChange = (option) => {
		setSelectedOption(option);
		switch (option) {
			case 'Opção 1':
				setChartData(dados1);
				break;
			case 'Opção 2':
				setChartData(dados2);
				break;
			case 'Opção 3':
				setChartData(dados3);
				break;
			case 'Opção 4':
				setChartData(dados4);
				break;
			case 'Opção 5':
				setChartData(dados5);
				break;
			default:
				setChartData(dados1);
		}
	};
	return (
		<ScrollView style={{ backgroundColor: 'white', flex: 1 }} vertical>
			<View style={styles.graphContainer}>
				<View style={styles.topContainer}>
					<View style={styles.leftContent} />
					<Dropdown onOptionChange={handleOptionChange} />
				</View>
				<View style={styles.chartContainer}>
					<ScrollView horizontal><Chart data={chartData} /></ScrollView>
					<ScrollView horizontal><Chart data={chartData} /></ScrollView>
					<ScrollView horizontal><Chart data={chartData} /></ScrollView>
				</View>
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
		width: 300,
	},
	chartContainer: {
		backgroundColor: 'white',
	},
})

export default Graph;
