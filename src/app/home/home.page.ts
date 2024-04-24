import { Component, OnInit } from '@angular/core';
import { Database, onValue, ref } from '@angular/fire/database';
import { LocalNotifications } from '@capacitor/local-notifications';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage implements OnInit {

  constructor(private database: Database) {
    
  }

  async ngOnInit() {
    await LocalNotifications.requestPermissions(); // Solicitar permisos de la app
    const route = ref(this.database, '/tiempo'); // Ruta directa a donde se encuentra el número

    onValue(route, (snapshot) => {
      const numero = snapshot.val(); // Obtén el valor numérico de la consulta
      const background = document.getElementById('background');

      if (background) {
        background.classList.add('fade-out'); // Agrega la clase para desvanecer la imagen actual
        setTimeout(async () => {
          // Cambia la imagen después de que la imagen actual se haya desvanecido
          if (numero >= 0 && numero <= 1000) {
            background.style.backgroundImage = "url(/assets/img/noche.jpg)";
            await this.enviarNotificacion("¡Buenas noches!", "Es hora de descansar");
          } else if (numero > 2500) {
            background.style.backgroundImage = "url(/assets/img/dia.jpg)";
            await this.enviarNotificacion("Buenos días", "Es hora de empezar el día");
          } else {
            background.style.backgroundImage = "url(/assets/img/tarde.jpg)";
            await this.enviarNotificacion("¡Buenas tardes!", "Es hora de aprovechar la tarde");
          }
          background.classList.remove('fade-out'); // Elimina la clase de desvanecimiento para mostrar la nueva imagen
        }, 500); // Ajusta este valor al mismo que la duración de la transición en CSS
      }
    });
  }

  async enviarNotificacion(title: string, body: string) {
  await LocalNotifications.schedule({
    notifications: [
      {
        title: title,
        body: body,
        id: Math.floor(Math.random() * 1000), // Generar un ID aleatorio para evitar conflictos
        schedule: {
          allowWhileIdle: true // Permite que la notificación se ejecute incluso durante el modo Reposo
        }
      }
    ]
  });
}
}
