#ifndef SRC_UTILS_TEST_LECTORTEST_HPP_
#define SRC_UTILS_TEST_LECTORTEST_HPP_

using namespace std;
#include <iostream>
#include <string>

#include <stdio.h>

#include "../../Server/lector/Lector.hpp"
#include "Assert.hpp"
class LectorTest{

	public:
		static void ejecutar(Assert* testSuite);
		static void test01CuandoLePedisAlLectorQueLeaTraeElAncho(Assert* testSuite);
		static void test02CuandoLePedisAlLectorQueLeaTraeElAlto(Assert* testSuite);
		static void test03CuandoLePedisAlLectorElArchivoSeLeeCorrectamente(Assert* testSuite);
		static void test04CuandoLePedisAlLectorElArchivoSeLeeCorrectamenteNoTraeMensajesDeError(Assert* testSuite);
		static void test05CuandoLeEnviasUnArchivoQueNoExisteSeLanzaUnError(Assert* testSuite);
		static void test06CuandoLeEnviasUnArchivoMalFormateadoSeLanzaUnError(Assert* testSuite);
		static void test07CuandoLeEnviasUnArchivoMalElAnchoSeCargaElDefaultYSeLanzaUnError(Assert* testSuite);
		static void test08CuandoLeEnviasUnArchivoMalElAltooSeCargaElDefaultYSeLanzaUnError(Assert* testSuite);
};





#endif /* SRC_UTILS_TEST_LECTORTEST_HPP_ */
