import React, { useEffect, useState } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList  } from '../components/components';
import { reading } from '../routes/routes';
import staticData from "../../dataTest/data1.json"
import alternativeData from "../../dataTest/data2.json"

function Home({ navigation, onDataChange }) {//	Descomentar quando para usar o back-end efetivamente
	// const [data, setData] = useState(reading());
	const [data, setData] = useState(staticData);

	let dataHora = data[0].datetime.split(' ');
  return (
    <ScrollView>
      {data.map((item, index) => (
        <View key={index}>
          {index === 0 && (
						<View>
              <HorizontalList Variavel="Data" Valor={dataHora[0]} Input={false} />
              <HorizontalList Variavel="Hora" Valor={dataHora[1]} Input={false} />
            </View>
          )}
					<HorizontalList Variavel={item.type} Valor={item.value} Input={false} />
        </View>
      ))}
      <View style={styles.roundButtonContainer}>
        <View style={styles.leftContent} />
        <TouchableOpacity style={styles.roundButton} onPress={() => setData(alternativeData)}>
          <Text style={styles.textContent}>{'Nova Leitura'}</Text>
        </TouchableOpacity>
      </View>
    </ScrollView>
  );
}

const styles = StyleSheet.create({
	baseHomeContainer: {
		backgroundColor: '#FFFFFF',
		flex: 1,
	},
	Button:{
		backgroundColor: '#5DB075',
    borderRadius: 10,
    paddingVertical: 10,
    paddingHorizontal: 20,
    marginTop: 20,
	},
	roundButtonContainer: {
    flex: 1,
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-between',
    paddingHorizontal: 20,
  },
  roundButton: {
    borderRadius: 20,
    paddingVertical: 10,
    paddingHorizontal: 20,
    marginTop: 20,
		backgroundColor:'#5DB075'
  },
	leftContent: {
    flexDirection: 'row',
    alignItems: 'center',
  },
	textContent:{
		color: 'white', 
		fontWeight: 'bold'
	}
});

export default Home;