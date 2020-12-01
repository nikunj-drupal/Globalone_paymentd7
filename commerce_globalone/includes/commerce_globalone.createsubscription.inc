<?php

class CommerceGlobaloneCreateSubscription {

  public $_terminal;
  public $_cardSaveTime;
  public $mode;
  public $_paymentPara;
  public $_securecardref;
  public $_card_ref;
  public $_storesub_ref;
  public $_storesub_date;
  public $_subinfo;

  public function __construct($term,$paymentPara,$subinfo=array()) {
    $this->_terminal = $term;
    $this->mode = $term['mode'];
    $this->_paymentPara = $paymentPara;
    $this->_cardSaveTime = date('d-m-Y:H:i:s').':000';
    $this->_subinfo = $subinfo;
  }

  public function createHash() {

    //TERMINALID+MERCHANTREF+STOREDSUBSCRIPTIONREF+SECURECARDMERCHANTREF+DATETIME+STARTDATE+SECRET
    //$params = $this->_paymentPara;
    $stringToHash = '';
    $stringToHash .= $this->_terminal['terminal_id'];
    $stringToHash .= $this->_terminal['terminal_id'].''.time();
    $stringToHash .= $this->_subinfo['storesub_mref'];
    $stringToHash .= $this->_subinfo['seccard_mref'];
    $stringToHash .= $this->_cardSaveTime;
    //$stringToHash .= '01-02-2019';
    $stringToHash .= date('d-m-Y');
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

  public function createSubscription() {

      $format = new CommerceGlobaloneCreateSubscriptionFormat($this->_terminal,$this->_cardSaveTime,$this->_subinfo,$this->createHash());
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