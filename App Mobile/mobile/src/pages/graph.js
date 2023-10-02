import React from 'react';
import { View, StyleSheet, Text, Button } from 'react-native';
import { Chart, Dropdown } from '../components/components';

function Graph({ navigation }) {
	return (
		<View style={{ backgroundColor: 'white', flex: 1 }}><View style={styles.graphContainer}>
			<View style={styles.topContainer}>
				<View style={styles.leftContent}/>
				<Dropdown/>
			</View>
			<View style={styles.chartContainer}><Chart/></View>
		</View></View>

	);
}

const styles = StyleSheet.create({
	graphContainer:{
		flexDirection: 'column',
		backgroundColor: 'white',
	},
	topContainer:{
		height: '80',
		flexDirection: 'row',
		justifyContent: 'space-between',
	},
	leftContent: {
		flexDirection: 'row',
		alignItems: 'center',
		width: 300,
	},
	chartContainer:{
		backgroundColor: 'white',
	},
})

export default Graph;
