import React from 'react';
import { View, Text, TouchableOpacity, StyleSheet } from 'react-native';

export const HorizontalList = ({Variavel, Valor}) => {
  return (
    <View style={styles.container}>
      <View style={styles.circle}></View>
      <View style={styles.leftContent}>
        <Text style={styles.boldText}>{Variavel}</Text>
      </View>
      <View>
        <Text>{Valor}</Text>
      </View>
    </View>
  );
};

export const HR = () => {
    return(
        <View style={styles.hr}></View>
    );
};
/*
export const RoundButtom = ({conteudo, navigation}) => {
    return(
        <View style={styles.container}/>
    );
};

RoundButtom.navigationOptions = ({ navigation }) => {
    return {
      headerRight: () => (
        <TouchableOpacity
          style={styles.roundButtom}
          onPress={() => {
            
          }}>
          <Text style={styles.textRoundButtom}>{conteudo}</Text>
        </TouchableOpacity>
      ),
    };
  };
*/
const styles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-between',
    width: '100%', 
    height: 50, 
    paddingHorizontal: 10, 
    backgroundColor: 'white', 
  },
  image: {
    width: 24, 
    height: 24, 
    marginRight: 10, 
  },
  leftContent: {
    flexDirection: 'row', 
    alignItems: 'center', 
  },
  boldText: {
    fontWeight: 'bold', 
  },
  circle: {
    width: 18, 
    height: 18, 
    backgroundColor: '#4B9460', 
    borderRadius: 18 / 1, 
    borderColor: 'darkgray', 
    borderWidth: 1, 
  },
  hr:{
    width: '90%', 
    height: 1, 
    backgroundColor: 'black', 
    alignSelf: 'center', 
  },
  /*roundButtom: {
    marginRight: 15,
    borderRadius: 10,
    paddingVertical: 5,
    paddingHorizontal: 10,
    backgroundColor: '#4B9460',
  },
  textRoundButtom: {
    color: 'white',
    fontWeight: 'bold',
  },*/
});