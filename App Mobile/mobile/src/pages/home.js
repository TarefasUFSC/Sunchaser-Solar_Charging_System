import React, { useEffect, useState } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList, RoundButton  } from '../components/components';
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
              <HorizontalList Variavel="Data" Unidade={dataHora[0]} Input={false} />
              <HorizontalList Variavel="Hora" Unidade={dataHora[1]} Input={false} />
            </View>
          )}
					<HorizontalList Variavel={item.type} Unidade={item.value} Input={false} />
        </View>
      ))}
      <RoundButton palavra='Nova Leitura' page='home' color = '#5DB075' tColor='white' onPressFunction={()=>setData(alternativeData)}/>
    </ScrollView>
  );
}

const styles = StyleSheet.create({});

export default Home;