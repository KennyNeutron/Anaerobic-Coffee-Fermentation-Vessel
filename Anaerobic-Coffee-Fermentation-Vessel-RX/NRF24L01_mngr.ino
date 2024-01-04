//NRF24L01 Manager

void nrf24L01_decrypt(uint8_t dataID, float dataParaData){
      switch(dataID){
        case id_SurfaceTemperature:
          SurfaceTemperature=dataParaData;
          break;
        case id_m5Humidity:
          m5Humidity=dataParaData;
          break;
        case id_m5Temperature:
          m5Temperature=dataParaData;
          break;
        case id_aht20Humidity:
          aht20Humidity=dataParaData;
          break;
        case id_aht20Temperature:
          aht20Temperature=dataParaData;
          break;
        case id_tdsValue:
          tdsValue=dataParaData;
          break;
        case id_pH4052C:
          pH4052C=dataParaData;
          break;
        default:
          delay(10);
          break;
      }

}