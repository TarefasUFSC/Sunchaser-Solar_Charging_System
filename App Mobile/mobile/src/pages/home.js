import React, { useEffect, useState, useContext } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList, RoundButton } from '../components/components';
import { ESP32Context } from '../../App'; // Importar o contexto

function Home() {

  const { batVolt, solarBatAmp, batLoadAmp, reloadData } = useContext(ESP32Context);

  return (
    <ScrollView>
      <RoundButton palavra='Nova Leitura' page='home' color='#5DB075' tColor='white' onPressFunction={() => (reloadData())} />

      {batVolt.length && solarBatAmp.length && batLoadAmp.length ?
        (<>
          <View >
            <HorizontalList Variavel="Data" Unidade={batVolt[0].datetime} Input={false} />
            <HorizontalList Variavel="Hora" Unidade={batVolt[0].datetime} Input={false} />

          </View>
          <Text>Existem {batVolt.length} leituras na memoria</Text>
          {batVolt ?
            (<View >
              <HorizontalList Variavel={"Tensão na Bateria"} Unidade={batVolt[0].value + "V"} Input={false} />
            </View>) :
            <><Text>Sem batVolt</Text></>}

          {solarBatAmp ?
            (<View >
              <HorizontalList Variavel={"Corrente entre o Painel e a Bateria"} Unidade={solarBatAmp[0].value + " A"} Input={false} />
            </View>) :
            <><Text>Sem SolarBat</Text></>}
          {batLoadAmp ?
            (<View >
              <HorizontalList Variavel={"Corrente entre a Bateria e a Carga"} Unidade={batLoadAmp[0].value + " A"} Input={false} />
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