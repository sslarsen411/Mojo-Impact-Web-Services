<?php 
/**
 * dbObj  
 * Universal DB handling for LWS Hosting
 * @author Scott Larsen
 * @copyright 2010 - 2014
 * 01-25-10 Added functionality to change databases
 * 06-12-13 Changed parameter order in queryDatabase: rowcount now last
 *          will default to FALSE. Most queries need only db, user, pass
 * 09-20-2014 Minor fixes
 * 07-19-2016 Added Create Tables function using multi-query
 * 08-10-2016 Changed file name to be more consistant
 * 06-24-2017 Changed name of file to mwd for migration to mojo web development
 * now in root
 */
class dbObj{     
    protected $conn;      
    /** Constructor
     * @return object mysqli connector
     */
    public function __construct($inUser,$inPass,$inDb,$inConfig=''){       
        if(empty($inConfig))
            $db_config = self::getConfigArray($inUser,$inPass,$inDb);
        else
            $db_config = $inConfig; // DB config array
        try{
            $this->conn = new mysqli($db_config['db_host'], 
                                     $db_config['db_user'], 
                                     $db_config['db_pass'], 
                                     $db_config['db_name'] );
            if ($this->conn->connect_error) {
                throw new Exception(
                    '<h3><span style="color:red">
                     MySQL Connection Database Error</span>: ' . 
                     mysqli_connect_error().'</h3>'
                );
    		}else{    	
    			return 	$this->conn;
    		}
        }catch (Exception $e){            
            exit($e->GetMessage()) ;
        }         
    } // END CONSTRUCTOR
    /**       
      * @return object mysqli connector       
      */
    public function getConn(){
        return $this->conn;
    }
    /** realEscape() 
      * @return mixed       
      */
    public static function realEscape($inVal, $inDb){        
        $conn = new dbObj($inDb);
        $db_conn =	$conn->conn;     
		$escaped = $db_conn->real_escape_string($inVal);
		$db_conn->close();
		return $escaped;
	}
     /**
      * dbObj::queryDatabase()
      * @param valid query string, DB config values or array      
      * @return mixed     
      */     
     public static function queryDatabase($qs     ,$inUser=''  , $inPass='',
                                          $inDb='',$inConfig='', $getRowCount=FALSE){                                          
        if (empty($inConfig))
            $conn = new dbObj($inUser,$inPass, $inDb);
        else
            $conn = new dbObj('','','',$inConfig);        
        $db_conn =	$conn->conn;     
        $rs = self::doQuery($qs, $db_conn,$getRowCount);
        $db_conn->close();
        return $rs;        
     }// END queryDatabase Function 
     /** queryDatabase()  
      * @param query string, mysqli connector      
      * @return mixed array    
      */
     protected static function doQuery($qs, $inConn, $getRowCount=FALSE){
        $dbAction = strtoupper(substr($qs, 0,strpos($qs,' ')));
        $dataArray = array();      
        try{	
            $dbRS = $inConn->query($qs);            
            if (!$dbRS) {                
                throw new Exception('<h3><span style="color:red">
                                     MySQL Query Database Error</span>: ' 
                                    . $inConn->errno.': ' . $inConn->error);
            }else{
                if($dbAction == 'SELECT' ){ // SELECT query
                    if($getRowCount)        //Include number of rows returned
                        $dataArray['rows'] = $dbRS->num_rows;                    
                    while (($row_data = $dbRS->fetch_assoc()) !== NULL)
                            $dataArray[] = $row_data;                                    
                    mysqli_free_result($dbRS);
                    return $dataArray;                          
                }else{                      // INSERT, UPDATE, DELETE query                 
                    if($dbAction == 'INSERT'){                      
                        $newID = $inConn->insert_id; 
                        return $newID; 
                    }else{                      
                       // $dataArray['changed'] = $inConn->affected_rows;
                       // $dataArray['success'] = $dbRS;
                       // return $dataArray;
                       return $inConn->affected_rows;
                        //return $dbRS;      
                    }            		
                }//inner else                               
            }//outer else
        }catch (Exception $e){ 
            exit($e->GetMessage()) ;
        }
    }// END doQuery Function 
    private static function getConfigArray($inUser='', $inPass='',
                                           $inDB = '', $host= 'localhost'){
        $configArr['db_host'] = $host;                
        $configArr['db_user'] = $inUser;
        $configArr['db_name'] = $inDB;
        $configArr['db_pass'] = $inPass;         
        return $configArr;     
    }
/**
      * dbObj::createTables()
      * @param use multi query to create tables    
      * @return bool     
      */     
     public static function createTables($qs,$inConfig){                                          
        if (empty($inConfig))
            echo "Multi query failed: (" . $mysqli->errno . ") " . $mysqli->error;
        else
        $conn = new dbObj('','','',$inConfig);        
        $db_conn =	$conn->conn; 
     
        if (! $rs = $db_conn->multi_query($qs))
            echo "Multi query failed: (" . $mysqli->errno . ") " . $mysqli->error;
        //var_dump($rs);
       
        $db_conn->close();
        return 'Tables created';        
    }// END createTables Function     
} /* END dbObj CLASS*/