using DataAccess;
using Entidades;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace Estadisticas
{
    public partial class Alarmas : System.Web.UI.Page
    {
        enum Alarma
        {
            Todas,
            Temperatura,
            Sonido,
            Proximidad
        }
        string topic_temperaturas = "/swa/alarma";

        DateTime FechaDesde
        {
            get
            {
                if (!string.IsNullOrEmpty(txtFechaDesde.Text))
                {
                    DateTime fecha = DateTime.Parse(txtFechaDesde.Text);
                    return fecha.AddHours(fecha.Hour * -1).AddMinutes(fecha.Minute * -1).AddSeconds(fecha.Second * -1);
                }


                return DateTime.Today;
            }
        }
        DateTime FechaHasta
        {
            get
            {
                if (!string.IsNullOrEmpty(txtFechaHasta.Text))
                {
                    DateTime fecha = DateTime.Parse(txtFechaHasta.Text);
                    return fecha.AddHours(23).AddMinutes(59).AddSeconds(59);
                }

                return DateTime.Today.AddHours(23).AddMinutes(59).AddSeconds(59);
            }
        }

        Alarma alarmaSeleccionada
        {
            get
            {
                switch (ddlTipoAlarma.SelectedValue)
                {
                    case "todas":
                        return Alarma.Todas;

                    case "temperatura":
                        return Alarma.Temperatura;

                    case "proximidad":
                        return Alarma.Proximidad;

                    case "sonido":
                        return Alarma.Sonido;

                    default:
                        return Alarma.Todas;
                        
                }
            }
        }
        protected void Page_Load(object sender, EventArgs e)
        {
            if(!IsPostBack)
            {
                txtFechaDesde.Text = DateTime.Today.ToShortDateString();
                txtFechaHasta.Text = DateTime.Today.ToShortDateString();
            }
            

        }
        private List<Mensaje> obtenerTemperaturas(string topic_temperaturas, DateTime fechaDesde, DateTime fechaHasta)
        {
            return MensajeDataAccess.GetByTopic(topic_temperaturas, fechaDesde, fechaHasta);
        }

        protected void btnBuscar_Click(object sender, EventArgs e)
        {
            getDataTable();

            grdMensajes.PageIndex = 0;
            grdMensajes.DataBind();
        }

        private void getDataTable()
        {
            List<Mensaje> mensajes = obtenerTemperaturas(topic_temperaturas, FechaDesde, FechaHasta);

            filtrarAlarmas(alarmaSeleccionada, ref mensajes);

            grdMensajes.DataSource = mensajes;
            grdMensajes.DataBind();
        }

        private void filtrarAlarmas(Alarma alarmaSeleccionada, ref List<Mensaje> mensajes)
        {
            if(mensajes != null && mensajes.Count > 0)
            {
                if(alarmaSeleccionada != Alarma.Todas)
                {
                    switch (alarmaSeleccionada)
                    {
                        case Alarma.Temperatura:
                            mensajes = mensajes.Where(x => x.mensaje.ToLower().Contains("temperatura")).ToList();
                            break;
                        case Alarma.Sonido:
                            mensajes = mensajes.Where(x => x.mensaje.ToLower().Contains("sonido")).ToList();
                            break;
                        case Alarma.Proximidad:
                            mensajes = mensajes.Where(x => x.mensaje.ToLower().Contains("proximidad")).ToList();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        protected void grdMensajes_PageIndexChanging(object sender, GridViewPageEventArgs e)
        {
            getDataTable();

            grdMensajes.PageIndex = e.NewPageIndex;

            grdMensajes.DataBind();
        }
    }
}