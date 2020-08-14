<?php
	use PHPMailer\PHPMailer\PHPMailer;
    use PHPMailer\PHPMailer\Exception;
    
    require 'phpMailer/src/Exception.php';
    require 'phpMailer/src/PHPMailer.php';
    require 'phpMailer/src/SMTP.php';
    require_once('mim.config.inc');
    
   
    class miMailer extends PHPMailer{
    
    /**
     * myPHPMailer constructor.
     *
     * @param bool|null $exceptions
     * @param string    $body A default HTML message body
     */
        public function __construct($exceptions, $body = '')    {
            //Don't forget to do this or other things may not be set correctly!
            parent::__construct($exceptions);            
            $this->Host = 'mail.mojoimpact.com';  // Specify main and backup SMTP servers
            $this->SMTPAuth = true;                               // Enable SMTP authentication
            $mail->Username = 'mojo@mojoimpact.com';                 // SMTP username
            $this->Password = 'JdKpf5lMH4nL';                           // SMTP password
            $this->SMTPSecure = 'ssl';                            // Enable TLS encryption, `ssl` also accepted
            $mail->Port = 465;  
           // $this->Sender  = "mojo@mojoimpact.com";
            //$this->addReplyTo('mojo@mojoimpact.com','Mojo Impact');
            $this->msgHTML($body, './images/');
            $this->SMTPDebug = 1;
            $this->Debugoutput = function ($str, $level) {
               // echo "Debug level $level; message: $str\n";
            };
    }
    
        public function setFromName($inEmail,$fname){ 
            $this->setFrom($inEmail, $fname);            
        }
        public function setTo($inName){                  
            $this->addAddress("$inName@".ROOT_DOMAIN); 
        }
        public function sendToMe(){                  
            $this->addAddress('scott@mojoimpact.com', 'Scott Larsen'); 
        }  
          
    }

?>