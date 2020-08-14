<?php
//Thanks for iYETER on http://stackoverflow.com/questions/927341/upload-entire-directory-via-php-ftp
/**
 * 06-24-2017 Changed name of file to mwd for migration to mojo web development
 */
class mimFTP {
	private $connectionID;
	private $ftpSession = false;
	private $blackList = array('.', '..', 'Thumbs.db');
	public function __construct($ftpHost = "") {
		if ($ftpHost != "") $this->connectionID = ftp_ssl_connect($ftpHost);
     //   if ($ftpHost != "") $this->connectionID = ftp_connect($ftpHost,22); // 07/08/2016 CHANGE TO SFTP
	}

	public function __destruct() {
		$this->disconnect();
	}

	public function connect($ftpHost) {     
		$this->disconnect();
		$this->connectionID = ftp_ssl_connect($ftpHost);
     //   $this->connectionID = ftp_connect($ftpHost,22); // SFTP
		return $this->connectionID;
	}

	public function login($ftpUser, $ftpPass) {
		if (!$this->connectionID) throw new Exception("Connection not established.", -1);
		$this->ftpSession = ftp_login($this->connectionID, $ftpUser, $ftpPass);
		return $this->ftpSession;
	}

	public function disconnect() {
		if (isset($this->connectionID)) {
			ftp_close($this->connectionID);
			unset($this->connectionID);
		}
	}
    public function send_recursive_directory($localPath, $remotePath) {
        return $this->recurse_directory($localPath, $localPath, $remotePath);
    }
    private function recurse_directory($rootPath, $localPath, $remotePath) {
  //  echo "Current director: " . ftp_pwd($this->connectionID) ."<br />";
        $errorList = array();
    //    if (!is_dir($localPath)) throw new Exception("Invalid directory: $localPath");
        chdir($localPath);
        $directory = opendir(".");
        while ($file = readdir($directory)) {
            if (in_array($file, $this->blackList)) continue;
            if (is_dir($file)) {
                $errorList["$remotePath/$file"] = $this->make_directory("$remotePath/$file");
                echo "puts: $remotePath/$file<br />";
                ftp_chdir($this->connectionID,  "$remotePath/$file");
                $errorList[] = $this->recurse_directory($rootPath, "$localPath/$file", "$remotePath/$file");            
                chdir($localPath);            
            } else {
               // $errorList["$remotePath/$file"] = $this->put_file("$localPath/$file", "$remotePath/$file");  
              // echo "Current puts directory: " . ftp_pwd($this->connectionID) . "<br />";
              //echo "$file<br />";
                ftp_chdir($this->connectionID,  "$remotePath");
                $errorList["$remotePath/$file"] = $this->put_file("$localPath/$file", "$file");
            }
        }
        return $errorList;
    }
    public function make_directory($remotePath) {
        $error = "";
        try {
            $error = ftp_mkdir($this->connectionID, $remotePath);
        } catch (Exception $e) {
            if ($e->getCode() == 2) $error = $e->getMessage(); 
        }
        return $error;
    }
    public function put_file($localPath, $file) {
        $error = "";
        try {
            echo "Current puts: " . ftp_pwd($this->connectionID) . "<br />";
           // echo "$localPath, $file<br />";
            ftp_put($this->connectionID, $file, $localPath, FTP_BINARY); 
        } catch (Exception $e) {
            if ($e->getCode() == 2) $error = $e->getMessage(); 
        }
        return $error;
    }
}