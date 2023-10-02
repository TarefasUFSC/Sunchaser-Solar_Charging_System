import React from 'react';
import { Image, Button, StyleSheet } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import Home from './src/pages/home';
import Settings from './src/pages/settings';
import Graph from './src/pages/graph';
import { RoundButtom } from './src/components/components';

const Tab = createBottomTabNavigator();

function App() {
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
              <Button
                onPress={() => alert('This is a button!')}
                title="Nova Leitura"
                color="#4B9460"
              />
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