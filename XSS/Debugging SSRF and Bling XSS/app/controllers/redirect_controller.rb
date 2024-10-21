require_relative '../controller'

class RedirectController < Controller
  def get
    log_request format('Redirected request to %s', params['url'])

    res.set_redirect WEBrick::HTTPStatus::TemporaryRedirect, params['url']
  end
end
