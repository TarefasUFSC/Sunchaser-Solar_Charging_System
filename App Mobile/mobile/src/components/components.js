import React, { useState } from 'react';
import { View, Text, TextInput, TouchableOpacity, StyleSheet } from 'react-native';
import { BarChart } from 'react-native-chart-kit';

import dados from '../../dados.json';

export const HorizontalList = ({ Variavel, Valor, Input }) => {
	const [text, setText] = useState('');
	const handleTextChange = (newText) => {
		const numericText = newText.replace(/[^0-9]/g, '');
		setText(numericText);
	};
	return (
		<View>{Input ? (
			<View style={styles.container}>
				<View style={styles.leftContent}>
					<View style={styles.circle}></View>
					<Text style={styles.boldText}>{Variavel}</Text>
				</View>
				<View style={styles.inputContainer}>
					<TextInput
						style={[styles.input, { textAlign: 'center' }]}
						value={text}
						onChangeText={handleTextChange}
						keyboardType="numeric"
					/>
					<Text>{Valor}</Text>
				</View>
			</View>
		) : (
			<View style={styles.container}>
				<View style={styles.leftContent}>
					<View style={styles.circle}></View>
					<Text style={styles.boldText}>{Variavel}</Text>
				</View>
				<Text>{Valor}</Text>
			</View>
		)}
		</View>
	);
};

export const HR = () => {
	return (<View style={styles.hr}></View>);
};

export const Chart = () => {
	const data = {
		labels: dados.map((item) => item.label),
		datasets: [{ data: dados.map((item) => item.value) }]
	};
	const xAxisStyle = {
		fontSize: 12, // Tamanho da fonte
		transform: [{ rotate: '90deg' }], // Rotação de 90 graus
		marginLeft: 20, // Espaçamento à esquerda para acomodar a rotação
	};
	return (
		<View>
			<BarChart
				data={data}
				width={400}
				height={200}
				yAxisLabel=""
				chartConfig={{
					backgroundGradientFrom: 'white',
					backgroundGradientTo: 'white',
					decimalPlaces: 0,
					color: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`,
					labelColor: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`,
					barPercentage: 0.5,
					propsForLabels: {
						fontSize: 12, // Tamanho da fonte do eixo Y
					},
				}}
			/>
		</View>
	);
};

export const RoundButton = ({palavra}) => {
return(
		<View style={styles.roundButtonContainer}>
			<View style={styles.leftContent}/>
		  	<TouchableOpacity
				style={styles.roundButton}
				onPress={()=>{alert('Configs salvas (sqn)')}}
		  	>
				<Text style={styles.textRoundButton}>{palavra}</Text>
			</TouchableOpacity>
		</View>
	);
};

const styles = StyleSheet.create({
	container: {
		flexDirection: 'row',
		alignItems: 'center',
		justifyContent: 'space-between',
		width: '100%',
		height: 50,
		paddingHorizontal: 10,
		backgroundColor: 'white',
	},
	leftContent: {
		flexDirection: 'row',
		alignItems: 'center',
	},
	boldText: {
		fontWeight: 'bold',
	},
	circle: {
		width: 18,
		height: 18,
		backgroundColor: '#4B9460',
		borderRadius: 18 / 1,
		borderColor: 'darkgray',
		borderWidth: 1,
		marginRight: 10,
	},
	hr: {
		width: '95%',
		height: 1,
		backgroundColor: 'black',
		alignSelf: 'center',
	},
	inputContainer: {
		flexDirection: 'row',
		alignItems: 'center',
		width: 70
	},
	input: {
		width: 30,
		borderWidth: 1,
		borderColor: 'gray',
		padding: 1,
		fontSize: 18,
	},
	roundButtonContainer:{
		flex: 1,
		flexDirection: 'row', 
		alignItems: 'center', 
		justifyContent: 'space-between', 
		paddingHorizontal: 20, 
	},
	roundButton: {
		backgroundColor: '#4B9460', 
		borderRadius: 10, 
		paddingVertical: 10,
		paddingHorizontal: 20,
		marginTop: 20,
		alignSelf:'right'
	},
	textRoundButton: {
	  color: 'white',
	  fontWeight: 'bold',
	},
});