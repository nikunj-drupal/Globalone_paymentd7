<?php

class CommerceGlobaloneSecCard {

  public $_terminal;
  public $_cardSaveTime;
  public $mode;
  public $_paymentPara;

  public function __construct($term,$paymentPara) {
    $this->_terminal = $term;
    $this->mode = $term['mode'];
    $this->_paymentPara = $paymentPara;
    $this->_cardSaveTime = date('d-m-Y:H:i:s').':000';
  }

  public function createHash() {

    $params = $this->_paymentPara;
    $stringToHash = '';
    $stringToHash .= $this->_terminal['terminal_id'];
    $stringToHash .= $this->_terminal['terminal_id'].''.time();
    $stringToHash .= $this->_cardSaveTime;
    $stringToHash .= $params['CARDNUMBER'];
    $stringToHash .= $this->cleanExpiryDate($params['MONTH'],$params['YEAR']);
    $stringToHash .= $params['CARDTYPE'];
    $stringToHash .= $params['CARDHOLDERNAME'];
    $stringToHash .= $this->_terminal['secret'];

    $this->_postHash = md5($stringToHash);
    return md5($stringToHash);
  }

  private function cleanExpiryDate($month = '',$year = ''){
    if(strlen($year)>2):
          $year = substr($year, 2,2);
    endif;

    $date = $month.$year;

    return $date;
  }

  public function sendRegister() {
     $format = new CommerceGlobalonecardFormat($this->_paymentPara,$this->_terminal,$this->createHash(),$this->_cardSaveTime);
      $this->_normalizedPaymentParams = $format->getNormalizedPaymentParams();
      $XML = $format->getXML();
      // if ($this->_logRequest) {
      //   watchdog('commerce_globalone', 'GlobalONE request to @url: !param', array('@url' => $this->_terminal['url'], '!param' => '<pre>' . check_plain(print_r($XML, TRUE)) . '</pre>'), WATCHDOG_DEBUG);
      // } 

      $curl = new CommerceGlobaloneCurl();
      $resp = $curl->curlXmlRequest($this->_terminal['url'], $XML);

      return $resp;
  }

  public function controlResponseHash($responseHash) {
    if(isset($responseHash['ERRORSTRING'])) {
      return false;
    }
    else {
    $this->_normalizedPaymentReponse=$responseHash;
    return ($this->buildResponseHash() == $responseHash['HASH']);
    }
  }
  public function buildResponseHash() {
    $reponse = $this->_normalizedPaymentReponse;
    $payment = $this->_paymentParams;
    $stringToHash = '';
    $stringToHash .= $this->_terminal['terminal_id'];
    $stringToHash .= $reponse['UNIQUEREF'];
    $stringToHash .= $payment['AMOUNT'];
    $stringToHash .= $reponse['DATETIME'];
    $stringToHash .= $reponse['RESPONSECODE'];
    $stringToHash .= $reponse['RESPONSETEXT'];
    $stringToHash .= $this->_terminal['secret'];
    $this->_responseHash = md5($stringToHash);
    return md5($stringToHash);
  }
}