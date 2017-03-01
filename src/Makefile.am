newincludedir = $(prefix)/include/libopenikev2

# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
lib_LTLIBRARIES = libopenikev2.la
libopenikev2_la_SOURCES = alarm.cpp alarmable.cpp alarmcommand.cpp \
	alarmcontroller.cpp alarmcontrollerimpl.cpp attribute.cpp attributemap.cpp \
	authenticator.cpp autolock.cpp autovector.cpp busevent.cpp buseventchildsa.cpp \
	buseventcore.cpp buseventikesa.cpp busobserver.cpp bytearray.cpp bytebuffer.cpp \
	childsa.cpp childsacollection.cpp childsaconfiguration.cpp childsarequest.cpp \
	cipher.cpp closeikesacommand.cpp command.cpp condition.cpp configuration.cpp \
	configurationattribute.cpp cryptocontroller.cpp cryptocontrollerimpl.cpp diffiehellman.cpp \
	eappacket.cpp enums.cpp eventbus.cpp exitikesacommand.cpp generalconfiguration.cpp \
	id.cpp idtemplate.cpp ikesa.cpp ikesaconfiguration.cpp ikesacontroller.cpp \
	ikesacontrollerimpl.cpp ipaddress.cpp ipseccontroller.cpp ipseccontrollerimpl.cpp keyring.cpp \
	log.cpp logimpl.cpp message.cpp messagereceivedcommand.cpp mutex.cpp \
	networkcontroller.cpp networkcontrollerimpl.cpp networkprefix.cpp notifycontroller.cpp \
	notifycontroller_authentication_failed.cpp notifycontroller_cookie.cpp \
	notifycontroller_http_cert_lookup_supported.cpp notifycontroller_internal_address_failure.cpp \
	notifycontroller_invalid_ke.cpp notifycontroller_invalid_syntax.cpp \
	notifycontroller_no_additional_sas.cpp notifycontroller_no_proposal_chosen.cpp notifycontroller_rekey_sa.cpp \
	notifycontroller_single_pair_required.cpp notifycontroller_ts_unacceptable.cpp \
	notifycontroller_unsupported_critical_payload.cpp notifycontroller_use_transport_mode.cpp payload.cpp payload_auth.cpp \
	payload_cert.cpp payload_cert_req.cpp payload_conf.cpp payload_del.cpp payload_eap.cpp \
	payload_id.cpp payload_idi.cpp payload_idr.cpp payload_ke.cpp payload_nonce.cpp \
	payload_notify.cpp payload_sa.cpp payload_sk.cpp payload_ts.cpp payload_tsi.cpp \
	payload_tsr.cpp payload_vendor.cpp payloadfactory.cpp peerconfiguration.cpp \
	printable.cpp proposal.cpp pseudorandomfunction.cpp random.cpp semaphore.cpp \
	senddeletechildsareqcommand.cpp senddeleteikesareqcommand.cpp sendeapcontinuereqcommand.cpp \
	sendeapfinishreqcommand.cpp sendikeauthreqcommand.cpp sendikesainitreqcommand.cpp \
	sendinformationalreqcommand.cpp sendnewchildsareqcommand.cpp sendrekeychildsareqcommand.cpp \
	sendrekeyikesareqcommand.cpp socketaddress.cpp threadcontroller.cpp threadcontrollerimpl.cpp \
	trafficselector.cpp transform.cpp transformattribute.cpp utils.cpp \
	 aaasender.cpp  aaacontroller.cpp  aaacontrollerimpl.cpp \
        boolattribute.cpp stringattribute.cpp int32attribute.cpp radiusattribute.cpp

newinclude_HEADERS = alarm.h alarmable.h alarmcommand.h alarmcontroller.h \
	alarmcontrollerimpl.h attribute.h attributemap.h authenticator.h autolock.h autovector.h \
	busevent.h buseventchildsa.h buseventcore.h buseventikesa.h busobserver.h \
	bytearray.h bytebuffer.h childsa.h childsacollection.h childsaconfiguration.h \
	childsarequest.h cipher.h closeikesacommand.h command.h condition.h configuration.h \
	configurationattribute.h cryptocontroller.h cryptocontrollerimpl.h diffiehellman.h eappacket.h \
	enums.h eventbus.h exception.h exitikesacommand.h generalconfiguration.h id.h \
	idtemplate.h ikesa.h ikesaconfiguration.h ikesacontroller.h ikesacontrollerimpl.h \
	ipaddress.h ipseccontroller.h ipseccontrollerimpl.h keyring.h log.h logimpl.h \
	message.h messagereceivedcommand.h mutex.h networkcontroller.h \
	networkcontrollerimpl.h networkprefix.h notifycontroller.h \
	notifycontroller_authentication_failed.h notifycontroller_cookie.h notifycontroller_http_cert_lookup_supported.h \
	notifycontroller_internal_address_failure.h notifycontroller_invalid_ke.h notifycontroller_invalid_syntax.h \
	notifycontroller_no_additional_sas.h notifycontroller_no_proposal_chosen.h notifycontroller_rekey_sa.h \
	notifycontroller_single_pair_required.h notifycontroller_ts_unacceptable.h \
	notifycontroller_unsupported_critical_payload.h notifycontroller_use_transport_mode.h payload.h payload_auth.h \
	payload_cert.h payload_cert_req.h payload_conf.h payload_del.h payload_eap.h \
	payload_id.h payload_idi.h payload_idr.h payload_ke.h payload_nonce.h \
	payload_notify.h payload_sa.h payload_sk.h payload_ts.h payload_tsi.h payload_tsr.h \
	payload_vendor.h payloadfactory.h peerconfiguration.h printable.h proposal.h \
	pseudorandomfunction.h random.h semaphore.h senddeletechildsareqcommand.h \
	senddeleteikesareqcommand.h sendeapcontinuereqcommand.h sendeapfinishreqcommand.h \
	sendikeauthreqcommand.h sendikesainitreqcommand.h sendinformationalreqcommand.h \
	sendnewchildsareqcommand.h sendrekeychildsareqcommand.h sendrekeyikesareqcommand.h socketaddress.h \
	threadcontroller.h threadcontrollerimpl.h trafficselector.h transform.h \
	transformattribute.h utils.h   aaasender.h \
	boolattribute.h stringattribute.h int32attribute.h radiusattribute.h \
	aaacontroller.h  aaacontrollerimpl.h
libopenikev2_la_LDFLAGS = -version-info 0:7:0














