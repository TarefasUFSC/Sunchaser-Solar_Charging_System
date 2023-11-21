import React, { useEffect, useState } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList, RoundButton } from '../components/components';
import { getCache, getLTM } from '../routes/routes';
import staticData from "../../dataTest/data1.json";
import alternativeData from "../../dataTest/data2.json";

function Home({ navigation, onDataChange }) {
  const [batVolt, setBatVolt] = useState([1,1]);
  const [solarBatAmp, setSolarBatAmp] = useState([]);
  const [batLoadAmp, setBatLoadAmp] = useState([]);

  useEffect(() => {
    console.log("PORRRAAAA")
    let cache = getCache();
      let ltm = getLTM();

      console.log(cache);
    
  }, []);

  let dataHora = [1, 1];
  return (
    <ScrollView>
      {batVolt ? (
        batVolt.map((item, index) => (
          <View key={index}>
             {/* { {index === 0 && (
              <View>
                <HorizontalList Variavel="Data" Unidade={item.datetime} Input={false} />
                <HorizontalList Variavel="Hora" Unidade={item.datetime} Input={false} />
              </View>
            )}
            <HorizontalList Variavel={item.datetime} Unidade={item.value} Input={false} /> */} 
            <Text>oi</Text>
          </View>
        ))
      ) : (
        <></>
      )}
      <RoundButton palavra='Nova Leitura' page='home' color='#5DB075' tColor='white' onPressFunction={() => setBatVolt(getCache())} />
      {/* <RoundButton palavra='Nova Leitura' page='home' color='#5DB075' tColor='white' onPressFunction={() => setBatVolt(alternativeData)} /> */}
    </ScrollView>
  );
}

const styles = StyleSheet.create({});

export default Home;