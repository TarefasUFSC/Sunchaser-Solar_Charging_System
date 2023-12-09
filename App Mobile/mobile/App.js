import React, { useState, useEffect, createContext } from 'react';
import { Image, StyleSheet } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import axios from 'axios';

import Home from './src/pages/home';
import Graph from './src/pages/graph';
import Settings from './src/pages/settings';

export const ESP32Context = createContext();

const Tab = createBottomTabNavigator();

const App = () => {
  const [batVolt, setBatVolt] = useState([]);
  const [solarBatAmp, setSolarBatAmp] = useState([]);
  const [batLoadAmp, setBatLoadAmp] = useState([]);
  const [reloadDataFlag, setReloadDataFlag] = useState(false)
  const [isBootingUp, setIsBootingUp] = useState(true);

  useEffect(() => {
    reloadData()
    setIsBootingUp(false)
  }, []);


  // Este useEffect será executado após a atualização dos estados
  useEffect(() => {
    if (!isBootingUp) {
      console.log("Estados atualizados", { batLoadAmp, batVolt, solarBatAmp });

      getData();
    }
  }, [reloadDataFlag]);

  function reloadData() {
    setBatLoadAmp([]);
    setBatVolt([]);
    setSolarBatAmp([]);
    setReloadDataFlag(!reloadDataFlag)
    // getData() é chamado após os estados serem limpos
  }
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

  };
  const espContextValue = {
    batVolt,
    solarBatAmp,
    batLoadAmp,
    reloadData
  };



  return (
    <ESP32Context.Provider value={espContextValue}>
      <NavigationContainer>
        <Tab.Navigator>
          <Tab.Screen
            name="Home"
            component={Home}
            options={{
              headerTitle: "Leitura Mais Recente",
              tabBarLabel: 'Medições',
              tabBarIcon: ({ color, size }) => (
                <Image
                  source={require('./src/assets/home.png')}
                  style={{ width: size, height: size, tintColor: color }}
                />
              )
            }}
          />
          <Tab.Screen
            name="Graph"
            component={Graph}
            options={{
              headerTitle: "Gráficos",
              tabBarLabel: 'Gráficos',
              tabBarIcon: ({ color, size }) => (
                <Image
                  source={require('./src/assets/graph.png')}
                  style={{ width: size, height: size, tintColor: color }}
                />
              ),
            }}
          />
          <Tab.Screen
            name="Settings"
            component={Settings}
            options={{
              headerTitle: "Configurações do ESP",
              tabBarLabel: 'Configurações',
              tabBarIcon: ({ color, size }) => (
                <Image
                  source={require('./src/assets/setting.png')}
                  style={{ width: size, height: size, tintColor: color }}
                />
              ),
            }}
          />
        </Tab.Navigator>
      </NavigationContainer>
    </ESP32Context.Provider>
  );

}

const styles = StyleSheet.create({});

export default App;