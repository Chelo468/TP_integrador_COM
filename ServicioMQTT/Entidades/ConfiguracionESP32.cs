using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Entidades
{
    public class ConfiguracionESP32
    {
        public string alarma_temperatura { get; set; }
        public string temperatura_minima { get; set; }
        public string temperatura_maxima { get; set; }
        public string alarma_sonido { get; set; }
        public string alarma_proximidad { get; set; }

        public bool esAlarmaTemperaturaActivada()
        {
            return alarma_temperatura == "1";
        }

        public bool esAlarmaSonidoActivada()
        {
            return alarma_sonido == "1";
        }

        public bool esAlarmaProximidadActivada()
        {
            return alarma_proximidad == "1";
        }
    }
}