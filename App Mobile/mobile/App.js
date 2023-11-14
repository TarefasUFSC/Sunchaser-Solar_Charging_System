import React from 'react';
import { Image, StyleSheet} from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';

import Home from './src/pages/home';
import Graph from './src/pages/graph';
import Settings from './src/pages/settings';
import { RoundButton } from './src/components/components';

const Tab = createBottomTabNavigator();

const App = () => {
  return (
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
                style={{ width: size, height: size, tintColor: color}}
              />
            ),
            headerRight: () => (
              <RoundButton palavra='Nova Leitura' page='home' color = '#5DB075' tColor='white'/>
            ),
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
                style={{ width: size, height: size, tintColor: color}}
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
                style={{ width: size, height: size, tintColor: color}}
              />
            ),
          }}
        />
      </Tab.Navigator>
    </NavigationContainer>
  );
}

const styles = StyleSheet.create({});

export default App;