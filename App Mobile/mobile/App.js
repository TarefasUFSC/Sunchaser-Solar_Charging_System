import React, { useState, useEffect, createContext } from 'react';
import { Dimensions, Image, Modal, StyleSheet, Text, View } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import axios from 'axios';

import Home from './src/pages/home';
import Graph from './src/pages/graph';
import Settings from './src/pages/settings';
import { RoundButton } from './src/components/components';

export const ESP32Context = createContext();

const Tab = createBottomTabNavigator();

const App = () => {
  const [batVolt, setBatVolt] = useState([]);
  const [solarBatAmp, setSolarBatAmp] = useState([]);
  const [batLoadAmp, setBatLoadAmp] = useState([]);
  const [reloadDataFlag, setReloadDataFlag] = useState(false);
  const [isBootingUp, setIsBootingUp] = useState(true);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    reloadData();
    setIsBootingUp(false);
  }, []);
  // Este useEffect será executado após a atualização dos estados

  useEffect(() => {
    if (!isBootingUp) {

      getData();
      setIsLoading(false);
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
    await axios.get(url)
      .then(response => {
        data["page"] = response.data['page']
        data["total"] = response.data['total']
        data["items_per_page"] = response.data['qtd_per_page']
        data["bat_volt"] = (response.data['bat_volt']);
        data["bat_load"] = (response.data['bat_load_amp'])
        data["solar_bat"] = (response.data['sol_bat_amp'])
      })
      .catch(error => {
        console.error('Erro na chamada GET: ', error.data);
      });
    return data
  }

  async function loadData(type) {
    let data = await fetchData(type, 1)
    // append das listas nas variavei
    let page = parseInt(data["page"])
    let total = data["total"]
    let items_per_page = data["items_per_page"]
    let bat_volt = data["bat_volt"]
    let bat_load = data["bat_load"]
    let solar_bat = data["solar_bat"]

    if (page > 0) {
      setBatLoadAmp(currentBatLoadAmp => currentBatLoadAmp.concat(bat_load));
      setBatVolt(currentBatVolt => currentBatVolt.concat(bat_volt));
      setSolarBatAmp(currentSolarBatAmp => currentSolarBatAmp.concat(solar_bat));

      let pages_needed = total / (items_per_page * page)
      while (pages_needed > page) {
        page = page + 1;
        data = await fetchData(type, page)
        // append das listas nas variavei
        page = parseInt(data["page"])
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
              ),headerRight: () => (
                <View>
                  <RoundButton palavra='Nova Leitura' page='home' color='#5DB075' tColor='white' onPressFunction={() => reloadData()} />
                </View>
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
      {/* Modal de carregamento */}
      {isLoading && (
        <Modal transparent={true} animationType="fade">
          <View style={styles.modalContainer}>
            <View style={styles.modalContent}>
              <Text>LOADING!!</Text>
            </View>
          </View>
        </Modal>
      )}
    </ESP32Context.Provider>
  );
};

const styles = StyleSheet.create({
  // Estilos para o modal
  modalContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
  },
  modalContent: {
    backgroundColor: 'white',
    padding: 20,
    borderRadius: 10,
  },
});

export default App;