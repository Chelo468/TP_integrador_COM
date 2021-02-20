using DataAccess;
using Entidades;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace Estadisticas
{
    public partial class Temperatura : System.Web.UI.Page
    {
        string topic_temperaturas = "/swa/temperatura";

        DateTime FechaDesde
        {
            get
            {
                if(!string.IsNullOrEmpty(txtFechaDesde.Text))
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
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
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
            List<Mensaje> mensajes = obtenerTemperaturas(topic_temperaturas, FechaDesde, FechaHasta);

            grdMensajes.DataSource = mensajes;
            grdMensajes.DataBind();

            ScriptManager.RegisterStartupScript(this, this.GetType(), "crearDataTable", "doDataTable();", true);
        }
    }
}