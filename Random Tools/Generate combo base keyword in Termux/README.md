# PasTerm v0.0.1 (prebeta)

## Introducción

Genera combos a base de Keywords con Termux. Este proyecto tiene fines educativos y/o demostrativos.

Tenemos un grupo en facebook > https://www.facebook.com/groups/1175363389159546/

Video de instalacion > https://www.youtube.com/watch?v=0k9D20r2Q3U

## Novedades
> - Checkeo de version.

## Instalación

Pasterm [TERMUX]

Como todos preguntan "¿Qué es Termux?"
Bueno *Termux* es un emulador de terminal linux para android... lo mismo que en la pc pero mas barato.... [Termux está en la play store (gratis)]

> Hay que tener una vpn (obligatorio) ya que cada vez que saquemos un combo, nuestra ip quedara baneada temporalmente.

Una ves descargado TERMUX y el PASTERM hay que actualizar paquetes... De Termux
Y darle permiso a termux en nuestro terminal...

A los que no les pide actualizar paquetes ponen directo el 2do comando y aceptar (enter). 

1. apt update&&apt upgrade -y

2. termux-setup-storage

INSTALAMOS PHP y GIT para clonar el repositorio...

3. pkg install php git -y

4. git clone https://github.com/Juni0r007/PasTerm.git

5. ls

6. cd PasTerm

Ejecutamos el archivo *.php 

Antes de eso hay que darle permisos de ejecución al script

7. chmod +x pasterm.php;chmod +x pasterm.class.php

8. php pasterm.php

Les sale el generador de combos.

## Uso

Poner una keyword, de preferencia no tan larga seguida de comas > Ejemplo: @gmail.com,@hotmail.com,@aol.com

Y lo pegan, empezara el proceso y esperar a que les muestre la peticion para poner nombre al combo

> Una vez terminado el proceso solo debe poner el nombre del combo, seguido el script generara una ruta para poner en el checker de su preferencia.

Es obligatorio -por el momento- usar una vpn

¿Que es una cuenta Auto Pagable o AutoPay? 

Son cuentas  obtenidas con Sentry MBA y PasteBin [Termux pero no todas] en si estas son 100% reales por lo cual las paga una persona y si cambias la contraseña el dueño va cancelar la cuenta.

## Anexos

- Checker de Spotify en Termux > https://github.com/Juni0r007/SpotiChk
