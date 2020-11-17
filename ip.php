<?php
    require_once('mim.config.inc');
	$dns = dns_get_record(ROOT_DOMAIN, DNS_A);
    echo $dns[0]['ip'];  
?>