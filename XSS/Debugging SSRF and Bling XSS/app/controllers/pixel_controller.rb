require_relative '../controller'

class PixelController < Controller
  def get
    extra = CONFIG['callback_tokens'][params['callback_token']] || {}

    log_request 'HTML injection callback captured', vulnerability_data: extra

    res.content_type = 'image/png'
    res.body = IO.read('static/pixel.png')
  end
end
