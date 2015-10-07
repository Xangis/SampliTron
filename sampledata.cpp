#include "sampledata.h"

/**
* Constructor.
*/
SampleData::SampleData()
{
	_waveFile = NULL;
    _keyboardNote = 64;
    _sampleNote = 64;
    _volume = 127;
	_userSpecified = false;
	_looping = false;
#ifdef WIN32
	_sourceVoice = NULL;
	_buffer = NULL;
#endif
}

/**
* Destructor.  Cleans up pointers.
*/
SampleData::~SampleData()
{
	DestroySource();
	if( _waveFile != NULL )
	{
		delete _waveFile;
	}
}

/**
* Creates an XAudio2 source from the wavefile data.
*/
#ifdef WIN32
void SampleData::CreateSource(IXAudio2* xAudio2)
{
	if( _waveFile == NULL )
		return;
	if( _buffer != NULL || _sourceVoice != NULL )
	{
		DestroySource();
	}
	HRESULT hr = xAudio2->CreateSourceVoice( &_sourceVoice, _waveFile->GetWaveFormatEx(), XAUDIO2_VOICE_USEFILTER, 64.0, NULL, NULL, NULL );
	if( FAILED( hr ))
	{
		wxMessageBox(wxString::Format(_("Unable to create source voice: %d (%x) [%s]"), hr, hr, GetXAudioErrorString(hr) ));
	}
	_buffer = _waveFile->GetXAudio2Buffer();
	if( _looping )
		_buffer->LoopCount = XAUDIO2_LOOP_INFINITE;
}
#endif

/**
* Destroys the source associated with this sample and clears the audio data buffer.
*/
void SampleData::DestroySource()
{
#ifdef WIN32
	if( _buffer != NULL )
	{
		if( _buffer->pAudioData != NULL )
			delete[] _buffer->pAudioData;
		delete _buffer;
		_buffer = NULL;
	}
	if( _sourceVoice != NULL )
	{
		_sourceVoice->DestroyVoice();
		_sourceVoice = NULL;
	}
#endif
}

#ifdef WIN32
/**
* Gets the XAudio error string from an HRESULT.
*/
wxString SampleData::GetXAudioErrorString(HRESULT hr)
{
	switch( hr )
	{
		case XAUDIO2_E_INVALID_CALL:
			return _("Invalid XAudio2 API call.");
			break;
		case XAUDIO2_E_XMA_DECODER_ERROR:
			return _("The Xbox 360 XMA hardware suffered an unrecoverable error.");
			break;
		case XAUDIO2_E_XAPO_CREATION_FAILED:
			return _("An effect failed to instantiate.");
			break;
		case XAUDIO2_E_DEVICE_INVALIDATED:
			return _("An audio device became unusable through being unplugged or some other event.");
			break;
		default:
			return wxEmptyString;
	}
}
#endif
