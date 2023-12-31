import React, { useState } from 'react';
import { View, ScrollView, Dimensions, Text, TextInput, TouchableOpacity, StyleSheet, Modal } from 'react-native';
import { LineChart } from 'react-native-chart-kit';

let color = '#5DB075';

function setWidth(size, screenSize){
  const width = 40 * size;
  if (width < screenSize) return screenSize - 10;
  else return width
}

export const HorizontalList = ({ Variavel, Valor, Unidade, Input, setValor }) => {
  const handleTextChange = (newText) => {
    const numericText = newText.replace(/[^0-9]/g, '');
    setValor(numericText);
  };

  return (
    <View>
      {Input ? (
        <View style={styles.container}>
          <View style={styles.leftContent}>
            <View style={styles.circle}></View>
            <Text style={styles.boldText}>{Variavel}</Text>
          </View>
          <View style={styles.inputContainer}>
            <TextInput
              style={[styles.input, { textAlign: 'center' }]}
              value={Valor.toString()}
              onChangeText={handleTextChange}
              placeholder={Valor.toString()}
              keyboardType="numeric"
            />
            <Text>{Unidade}</Text>
          </View>
        </View>
      ) : (
        <View style={styles.container}>
          <View style={styles.leftContent}>
            <View style={styles.circle}></View>
            <Text style={{ fontWeight: 'bold' }}>{Variavel}</Text>
          </View>
          <Text>{Unidade}</Text>
        </View>
      )}
      <View style={styles.hr}></View>
    </View>

  );
};

export const Chart = ({ data, opt }) => {
  const chartData = {
    labels: data.map((item) => item.label),
    datasets: [{ data: data.map((item) => item.value) }]
  };
  const size = chartData.datasets[0].data.length;
  return (
    <View>
      <Text styles={styles.titleChart}>{opt}</Text>
      <LineChart
        data={chartData}
        width={setWidth(size, Dimensions.get("window").width)}
        height={220}
        yAxisSuffix={data[0].unit}
        chartConfig={{
          backgroundColor: "#e26a00",
          backgroundGradientFrom: color,
          backgroundGradientTo: "#7abd90",
          decimalPlaces: 1,
          color: (opacity = 1) => `rgba(255, 255, 255, ${opacity})`,
          labelColor: (opacity = 1) => `rgba(255, 255, 255, ${opacity})`,
          style: {
            borderRadius: 16
          },
          propsForDots: {
            r: "6",
            strokeWidth: "2",
            stroke: "#7abd90"
          },
          propsForLabels: {
            transform: [{ rotate: '-45deg' }],
          }
        }}
        bezier
        style={{
          marginVertical: 8,
          borderRadius: 16,
          marginHorizontal: 5
        }}
      />
    </View>
  );
};

export const RoundButton = ({ palavra, color, tColor, onPressFunction }) => {
  const buttonColor = { backgroundColor: color };
  const textColor = { color: tColor, fontWeight: 'bold' }
  return (
    <View style={styles.roundButtonContainer}>
      <View style={styles.leftContent} />
      <TouchableOpacity style={[styles.roundButton, buttonColor]} onPress={onPressFunction}>
        <Text style={textColor}>{palavra}</Text>
      </TouchableOpacity>
    </View>
  );
};

export const Dropdown = ({ onOptionChange }) => {
  const [isDropdownVisible, setIsDropdownVisible] = useState(false);
  const [selectedOption, setSelectedOption] = useState('tensão na Bateria');
  const handleOptionChange = (itemValue) => {
    setSelectedOption(itemValue);
    toggleDropdown();
    onOptionChange(itemValue);
  };
  const toggleDropdown = () => { setIsDropdownVisible(!isDropdownVisible); };

  return (
    <View style={styles.dropdownContainer}>
      <TouchableOpacity
        style={styles.picker}
        onPress={toggleDropdown}
      >
        <Text style={styles.itemStyle}>Trocar dados do Gráfico</Text>
      </TouchableOpacity>

      <Modal
        visible={isDropdownVisible}
        transparent={true}
        animationType="slide"
        onRequestClose={toggleDropdown}
      >
        <View style={styles.modalContainer}>
          {['Tensão na Bateria', 'Corrente entre o Painel e a Bateria', 'Corrente entre a Bateria e a Carga'].map((option) => (
            <TouchableOpacity
              key={option}
              style={styles.dropdownOption}
              onPress={() => handleOptionChange(option)}
            >
              <Text style={styles.itemStyle}>{option}</Text>
            </TouchableOpacity>
          ))}
        </View>
      </Modal>
    </View>
  );
};

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
  circle: {
    width: 18,
    height: 18,
    backgroundColor: color,
    borderRadius: 18 / 1,
    borderColor: 'darkgray',
    borderWidth: 1,
    marginRight: 10,
  },
  hr: {
    width: '95%',
    height: 1,
    backgroundColor: '#E8E8E8',
    alignSelf: 'center',
  },
  inputContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    width: 70
  },
  input: {
    width: 30,
    borderWidth: 1,
    borderColor: 'gray',
    padding: 1,
    fontSize: 18,
  },
  roundButtonContainer: {
    flex: 1,
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-between',
    paddingHorizontal: 20,
  },
  roundButton: {
    borderRadius: 20,
    paddingVertical: 10,
    paddingHorizontal: 20,
    marginTop: 20,
  },
  dropdownContainer: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    maxWidth: 150,
    paddingVertical: 10
  },
  picker: {
    borderRadius: 10,
    backgroundColor: color,
  },
  itemStyle: {
    color: 'white',
    paddingVertical: 10,
    paddingHorizontal: 20,
    fontSize: 16,
  },
  modalContainer: {
    flex: 1,
    justifyContent: 'flex-end',
  },
  dropdownOption: {
    backgroundColor: color,
    borderBottomColor: 'white',
    borderBottomWidth: 1,
  },
  titleChart:{
    fontWeight: 'bold', 
    fontSize: 16,
  }
});

export default styles;