import React, { useState } from 'react';
import { Image, StyleSheet } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { createStackNavigator } from '@react-navigation/stack';
import Home from './src/pages/home';
import Settings from './src/pages/settings';
import Graph from './src/pages/graph';
import Disconnected from './src/pages/disconnected';
import { RoundButton } from './src/components/components';

const Tab = createBottomTabNavigator();
const Stack = createStackNavigator();

function App() {
  const [isConnected, setIsConnected] = useState(false);
  if(isConnected){ return (
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
  );} 
  else{return(
    <NavigationContainer>
        <Stack.Navigator initialRouteName="Disconnected">
          <Stack.Screen
            name="Disconnected"
            component={Disconnected}
            options={{ headerShown: false }}
          />
        </Stack.Navigator>
      </NavigationContainer>
  );}
}

const styles = StyleSheet.create({});

export default App;