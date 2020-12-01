<?php

class CommerceGlobaloneSubscription {

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

    // $params = $this->_paymentPara;
    // $stringToHash = '';
    // $stringToHash .= $this->_terminal['terminal_id'];
    // $stringToHash .= '991081550132051';
    // $stringToHash .= time();
    // $stringToHash .= '01-02-2019';
    // $stringToHash .= $this->_terminal['secret'];
    //TERMINALID+MERCHANTREF+DATETIME+TYPE+NAME+PERIODTYPE+CURRENCY+RECURRINGAMOUNT+INITIALAMOUNT+LENGTH+SECRET

    $params = $this->_paymentPara;
    $stringToHash = '';
    $stringToHash .= $this->_terminal['terminal_id'];
    $stringToHash .= $this->_terminal['terminal_id'].''.time();
    $stringToHash .= $this->_cardSaveTime;
    $stringToHash .= 'AUTOMATIC';
    $stringToHash .= $params['CARDHOLDERNAME'];
    $stringToHash .= 'MONTHLY';
    $stringToHash .= 'USD';
    $stringToHash .= $params['RECURRINGAMOUNT'];
    $stringToHash .= $params['INITIALAMOUNT'];
    $stringToHash .= $params['LENGTH'];
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

  public function sendSubscription() {

      $format = new CommerceGlobaloneSubscriptionFormat($this->_paymentPara,$this->_terminal,$this->createHash(),$this->_cardSaveTime);
      $this->_normalizedPaymentParams = $format->getNormalizedPaymentParams();
      $XML = $format->getXML();

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