import React, { useEffect, useState, useContext } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList, RoundButton } from '../components/components';
import { ESP32Context } from '../../App'; // Importar o contexto

function Home() {

  function date(datetime, opt){
    let data = datetime.split('T')[0].replaceAll('-', '/')
    let time = datetime.split('T')[1].split('-')[0]
    return [data, time]
  }

  const { batVolt, solarBatAmp, batLoadAmp, reloadData } = useContext(ESP32Context);
  let datetime = date(batVolt[batVolt.length-1].datetime)
  
  return (
    <ScrollView>
      <RoundButton palavra='Nova Leitura' page='home' color='#5DB075' tColor='white' onPressFunction={() => (reloadData())} />
      {batVolt.length && solarBatAmp.length && batLoadAmp.length ?
        (<>
          <View >
            <HorizontalList Variavel="Data" Unidade={datetime[0]} Input={false} />
            <HorizontalList Variavel="Hora" Unidade={datetime[1]} Input={false} />
          </View>
          {batVolt ?
            (<View >
              <HorizontalList Variavel={"Tensão na Bateria"} Unidade={batVolt[batVolt.length-1].value + "V"} Input={false} />
            </View>) :
            <><Text>Sem batVolt</Text></>}

          {solarBatAmp ?
            (<View >
              <HorizontalList Variavel={"Corrente entre o Painel e a Bateria"} Unidade={solarBatAmp[solarBatAmp.length-1].value + " A"} Input={false} />
            </View>) :
            <><Text>Sem SolarBat</Text></>}
          {batLoadAmp ?
            (<View >
              <HorizontalList Variavel={"Corrente entre a Bateria e a Carga"} Unidade={batLoadAmp[batLoadAmp.length-1].value + " A"} Input={false} />
            </View>) :
            <><Text>Sem batLoad</Text></>}


        </>)
        :
        (<Text>Nenhuma Leitura</Text>)
      }
    </ScrollView>
  );
}

const styles = StyleSheet.create({});

export default Home;