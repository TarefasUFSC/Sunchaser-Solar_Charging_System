import React, { useState } from 'react';
import { View, Text, TextInput, TouchableOpacity, StyleSheet } from 'react-native';

export const HorizontalLisdt = ({Variavel, Valor, Input}) => {
  return (
    <View style={styles.container}>
        <View style={styles.leftContent}>
            <View style={styles.circle}></View>
            <Text style={styles.boldText}>{Variavel}</Text>
        </View>
        <View><Text>{Valor}</Text></View>
    </View>
  );
};

export const HorizontalList = ({Variavel, Valor, Input}) => {
    const [text, setText] = useState('');
    const handleTextChange = (newText) => {
        const numericText = newText.replace(/[^0-9]/g, '');
        setText(numericText);
    };
    return (
        <View>{Input ?(
            <View style={styles.container}>
                <View style={styles.leftContent}>
                    <View style={styles.circle}></View>
                    <Text style={styles.boldText}>{Variavel}</Text>    
                </View>
                <View style={styles.inputContainer}>
                    <TextInput
                        style={[styles.input,{textAlign: 'center'}]}
                        value={text}
                        onChangeText={handleTextChange}
                        keyboardType="numeric"
                    />
                    <Text>{Valor}</Text>
                </View>
            </View>
        ):(
            <View style={styles.container}>
                <View style={styles.leftContent}>
                    <View style={styles.circle}></View>
                    <Text style={styles.boldText}>{Variavel}</Text>
                </View>
                <Text>{Valor}</Text>
            </View>
        )}
        </View>
    );
};

export const HR = () => {
    return(<View style={styles.hr}></View>);
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
    marginRight: 10,
  },
  hr:{
    width: '95%', 
    height: 1, 
    backgroundColor: 'black', 
    alignSelf: 'center', 
  },
  inputContainer:{
    flexDirection: 'row',
    alignItems: 'center',
    width: 70
  },
  input: {
    width:30,
    borderWidth: 1,
    borderColor: 'gray',
    padding: 1,
    fontSize: 18,
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