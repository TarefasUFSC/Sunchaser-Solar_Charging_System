import React, { useEffect, useState, useContext } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import { HorizontalList } from '../components/components';
import { ESP32Context } from '../../App'; // Importar o contexto

function Home() {
  const { batVolt, solarBatAmp, batLoadAmp, reloadData, checkConnection } = useContext(ESP32Context);
  const flag = batVolt.length && solarBatAmp.length && batLoadAmp.length
  var datetime;

  useEffect(() => {
  }, []);

  function date(datetime) {

    checkConnection();
    let data = datetime.split('T')[0].replaceAll('-', '/')
    let time = datetime.split('T')[1].split('-')[0]
    return [data, time]
  }

  if (flag) datetime = date(batVolt[0].datetime)
  return (
    <ScrollView>
      {flag ?
        (<>
          <View >
            <HorizontalList Variavel="Data" Unidade={datetime[0]} Input={false} />
            <HorizontalList Variavel="Hora" Unidade={datetime[1]} Input={false} />
          </View>
          {batVolt ?
            (<View >
              <HorizontalList Variavel={"Tensão na Bateria"} Unidade={parseFloat(batVolt[0].value).toFixed(2) + "V"} Input={false} />
            </View>) :
            <><Text>Sem batVolt</Text></>}

          {solarBatAmp ?
            (<View >
              <HorizontalList Variavel={"Corrente entre o Painel e a Bateria"} Unidade={parseFloat(solarBatAmp[0].value).toFixed(2) + " mA"} Input={false} />
            </View>) :
            <><Text>Sem SolarBat</Text></>}
          {batLoadAmp ?
            (<View >
              <HorizontalList Variavel={"Corrente entre a Bateria e a Carga"} Unidade={parseFloat(batLoadAmp[0].value).toFixed(2) + " mA"} Input={false} />
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