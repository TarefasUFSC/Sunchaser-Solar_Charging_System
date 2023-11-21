import React, { useEffect, useState } from 'react';
import { View, Text, ScrollView, TouchableOpacity, StyleSheet } from 'react-native';
import axios from 'axios';
import { HorizontalList, RoundButton } from '../components/components';
import { getCache, getLTM } from '../routes/routes';
import staticData from "../../dataTest/data1.json";
import alternativeData from "../../dataTest/data2.json";

function Home({ navigation, onDataChange }) {
  const [cache, setCache] = useState({ "a": "b" });
  const [batVolt, setBatVolt] = useState([]);
  const [solarBatAmp, setSolarBatAmp] = useState([]);
  const [batLoadAmp, setBatLoadAmp] = useState([]);
  const [reloadDataFlag, setReloadDataFlag] = useState(false)

  useEffect(() => {
    console.log("PORRRAAAA")
    // getCache();  
    let ltm = getLTM();

    console.log(cache);

  }, []);

  let dataHora = [1, 1];

  function reloadData() {
    setBatLoadAmp([]);
    setBatVolt([]);
    setSolarBatAmp([]);
    setReloadDataFlag(!reloadDataFlag)
    // getData() é chamado após os estados serem limpos
  }

  // Este useEffect será executado após a atualização dos estados
  useEffect(() => {
    console.log("Estados atualizados", { batLoadAmp, batVolt, solarBatAmp });
    // Agora você pode chamar getData() aqui, se for necessário após a atualização dos estados
    getData();
  }, [reloadDataFlag]); // Dependências que, quando alteradas, acionam este useEffect


  async function fetchData(type, page) {
    let data = {
      'page': 0,
      'total': 0,
      "items_per_page": 0,
      "bat_volt": 0,
      "bat_load": 0,
      "solar_bat": 0
    }
    const url = "http://192.168.1.1/" + type + "?page=" + page
    console.log("url: ", url);
    await axios.get('http://192.168.1.1/cache?page=' + page).then(response => {
      console.log("deu bom na page ", page);
      data["page"] = response.data['page']
      data["total"] = response.data['total']
      data["items_per_page"] = response.data['qtd_per_page']
      data["bat_volt"] = (response.data['bat_volt']);
      data["bat_load"] = (response.data['bat_load_amp'])
      data["solar_bat"] = (response.data['sol_bat_amp'])
      // console.log(pg, total, items_per_page, bat_volt, bat_load, solar_bat);
    })
      .catch(error => {
        console.error('Erro na chamada GET: ', error.data);
        // setCache({ "error": error })
      });
    return data
  }


  async function loadData(type) {
    console.log("Pegando o ", type);
    let data = await fetchData(type, 1)
    // append das listas nas variavei
    let page = data["page"]
    let total = data["total"]
    let items_per_page = data["items_per_page"]
    let bat_volt = data["bat_volt"]
    let bat_load = data["bat_load"]
    let solar_bat = data["solar_bat"]

    console.log(page, total, items_per_page, bat_volt, bat_load, solar_bat);
    if (page > 0) {
      console.log("Leu");
      setBatLoadAmp(currentBatLoadAmp => currentBatLoadAmp.concat(bat_load));
      setBatVolt(currentBatVolt => currentBatVolt.concat(bat_volt));
      setSolarBatAmp(currentSolarBatAmp => currentSolarBatAmp.concat(solar_bat));

      let pages_needed = total / (items_per_page * page)
      while (pages_needed > page) {
        console.log("PRECISO DE MAIS PAGINASSSS");
        page = page + 1;
        data = await fetchData(type, page)
        // append das listas nas variavei
        page = data["page"]
        total = data["total"]
        items_per_page = data["items_per_page"]
        bat_volt = data["bat_volt"]
        bat_load = data["bat_load"]
        solar_bat = data["solar_bat"]
        if (page > 0) {
          setBatLoadAmp(currentBatLoadAmp => currentBatLoadAmp.concat(bat_load));
          setBatVolt(currentBatVolt => currentBatVolt.concat(bat_volt));
          setSolarBatAmp(currentSolarBatAmp => currentSolarBatAmp.concat(solar_bat));

        } else {
          break
        }
      }
    }
  }

  async function getData() {
    await loadData('cache')
    await loadData('ltm')

  }
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
              <HorizontalList Variavel={"Tensão na Bateria"} Unidade={batVolt[0].value + " V"} Input={false} />
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