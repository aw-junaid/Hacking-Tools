require_relative '../controller'

class CollectController < Controller
  def get
    @callback_token = params['callback_token']

    @url = format '%s://%s:%d/collect',
      req.ssl? ? 'https' : 'http',
      req.host,
      req.port

    log_request format('XSS callback served, callback set to %s', @url)

    res.content_type = 'text/javascript'
    res.body = ERB.new(IO.read('app/views/collect.js.erb')).result(binding)
  end

  def post
    extra = CONFIG['callback_tokens'][params['callback_token']] || {}

    log_request 'XSS callback captured', vulnerability_data: extra
  end
end
