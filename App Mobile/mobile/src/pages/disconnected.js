import React from 'react';
import { View, Text, TouchableOpacity, StyleSheet } from 'react-native';
import { BackHandler } from 'react-native';

function Disconnected({ navigation }) {
  return (
    <View style={styles.disconnectedContainer}>
        <View style={styles.container}>
            <View style={styles.titleContainer}>
                <Text style={styles.titleText}>Sem conexão</Text>
            </View>
            <View style={styles.textContainer}>
                <Text style={styles.textText}>
                    Não há nenhum dispositivo conectado atualmente. Gostaria de tentar uma conexão bluetooth agora? Lembramos que para acessar os dados faz-se necessária a conexão com o microcontrolador.
                </Text>
            </View>
            <View style={styles.buttonContainer}>
                <TouchableOpacity style={[styles.roundButton, {backgroundColor: '#5DB075'}]} onPress={() => { alert('CONECTA AE ONEGAI') }}>
                    <Text style={{color: 'white', fontWeight: 'bold', fontSize: 20}}>Tentar conexão</Text>
                </TouchableOpacity>
                <TouchableOpacity style={[styles.roundButton, {backgroundColor: 'white'}]} onPress={() => {BackHandler.exitApp();}}>
                    <Text style={{color: '#5DB075', fontWeight: 'bold', fontSize: 20}}>Fechar aplicativo</Text>
                </TouchableOpacity>
            </View>
        </View>
    </View>
  );
}

const styles = StyleSheet.create({
    disconnectedContainer:{
        backgroundColor: '#5DB075',
        flex: 1,
        alignItems: 'center',
        height: '100%',
        width: '100%',
        paddingTop: '45%'
    },
    container: {
        paddingVertical: 10,
        backgroundColor: 'white',
        alignItems: 'center',
        flexDirection: 'column',
        height:'60%',
        width: '90%',
        borderRadius: 10,
    },
    titleContainer:{
        paddingTop: 15,
        paddingBottom: 10,
        justifyContent: 'center'
    },
    titleText:{
        fontWeight: 'bold',
        fontSize: 40
    },
    textContainer:{
        paddingHorizontal: 20,
    },
    textText:{
        textAlign: 'center',
        color: '#666666',
        fontSize: 20
    },
    buttonContainer:{
        paddingTop: 10,
        width: '80%',
        flexDirection: 'column',
        alignItems: 'center'
    },   
    roundButton: {
        alignItems: 'center',
        width: '80%',
        height: '30%',
        borderRadius: 20,
        paddingVertical: 15
      },
});

export default Disconnected;
