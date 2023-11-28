import React, { useEffect, useState, useContext } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList, RoundButton } from '../components/components';
import { ESP32Context } from '../../App'; // Importar o contexto

function Home() {

  const { batVolt, solarBatAmp, batLoadAmp, reloadData } = useContext(ESP32Context);
  var date ='data', hour = 'hora', flag = false;
  if (batVolt.length>0){
    date = batVolt[-1].datetime.split('T')[0].replaceAll('-', '/').substr(2, batVolt[-1].datetime.length)
    hour = batVolt[-1].datetime.split('T')[1].split('-')[0]
    if(batVolt.length>0 && solarBatAmp.length>0 && batLoadAmp.length>0) flag = true;
  }
  else{
    date="00/00/0000"; hour="00:00"
  }
  return (
    <ScrollView>
      {flag ?
        (<>
          <View >
            <HorizontalList Variavel="Data" Unidade={date} Input={false} />
            <HorizontalList Variavel="Hora" Unidade={hour} Input={false} />
          </View>
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
        (<View style={{backgroundColor: 'white', height: 50}}><Text style={{marginHorizontal: 10, fontWeight: 'bold'}}>Nenhuma Leitura</Text></View>)
      }
    </ScrollView>
  );
}

const styles = StyleSheet.create({});

export default Home;