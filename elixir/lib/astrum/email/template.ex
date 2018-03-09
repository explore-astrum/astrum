defmodule Astrum.Email.Template do

  def confirm(token, email) do
    {
      "Confirm your email address with Astrum",
      "Hello! We just need to verify that #{email} is your email address.  Click the following link to login #{Astrum.Config.astrum_url()}/auth/verify/#{token}"
    }
  end

end